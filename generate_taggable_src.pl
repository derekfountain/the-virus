#!/usr/bin/perl -w
use strict;

# SP1 Game Framework
# Copyright (C) 2022 Derek Fountain
#
# v1.20
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

# This script creates a "taggable source" file which can be loaded into my
# modified version of Fuse. See https://spectrumcomputing.co.uk/forums/viewtopic.php?t=974
#
# The idea is to load the symbols of the generated executable and note the
# address in the Spectrum memory each one appears at. Then load and merge
# all the *.c.lis files produced by the compiler. Those *.c.lis files
# contain the assembly language generated to implement the C code, and
# each instruction is labelled with a numeric offset from the start of
# the list file. So the idea is to pick out each symbol as the parse
# of the list file comes across it, note the location in memory that
# symbol is at, then continue down the list file adding the offset of each
# instruction to the symbol's definition.
#
# Example:
#
#   748                          	SECTION code_compiler
#   749                          ;main.c:49: void main(void)
#   750                          ;	---------------------------------
#   751                          ; Function main
#   752                          ; ---------------------------------
#   753                          _main:
#   754  0000  dde5              	push	ix
#   755  0002  dd210000          	ld	ix,0
#
# I come across the _main: symbol, the first instruction of which is at
# offset 0000 in this list file (line 753). I find its location in the symbols
# table - let's say it lands at 32768. The first subsequent instruction is a
# "push", and its offset is 0000 from the start of this file (line 754). So
# that call is at 0000-0000+32768, which is 32768.
#
# The next instruction is the "ld ix,0" at offset 0002, so 0002-0000
# is 2, 32768+2 is 32770, so that instruction is at 32770. And so on.
#
# When a new symbol is found:
#
#   460                          	SECTION code_compiler
#   461                          ;player.c:46: void hide_player( void )
#   462                          ;	---------------------------------
#   463                          ; Function hide_player
#   464                          ; ---------------------------------
#   465                          _hide_player:
#   466                          ;player.c:48: player_x = 255;
#   467  0000  210000            	ld	hl,_player_x
#   468  0003  3600              	ld	(hl),0xff
#
# I find the _hide_player: symbol. Let's say it lands at 40000. The next
# instruction is at offset 0000, which places it at 40000; the next is
# at offset 0003 which places that one at 40003.
#
# The output from this script is in the form:
#
# 0x8000 ++ <rest of line in list file>
#
# This loads into Fuse, which tags the line with the address value it
# contains - 0x8000 in this case. When the PC reaches 8000 in the
# debugger, that value is searched in the text as tag, and the text
# viewer is scrolled to that location.
#
# Some locations will have several tags all the same:
#
#   724   008000 _main:
#   725                          ;main.c:39: if( is_rom_writable() )
#   726   008000 cd0000          	call	_is_rom_writable
#
# In this case the viewer would scroll to the /last/ line with that address
# entry which isn't as useful in the debugger. So the output is set to
# only write a single address once in the tagged output:
#
# 0x8000 ++ <rest of line in list file>
# 0x     ++ <rest of next line in list file>
#
# which leaves the tag (0x8000) at the first of the group. That's therefore
# the one the debugger text scrolls to. That's what the addresses_written
# thing is about.




# Load in the symbols table. The real symbols table has all the information
# but for now my previously created symbols file containing name=value pairs
# is easiest to parse.
#
my %symbols = ();

my $symbols_filename = shift( @ARGV );
open( SYM_FILE_HANDLE, $symbols_filename ) or die("No such input file \"$symbols_filename\"\n");
while( my $line = <SYM_FILE_HANDLE> ) {

  if( $line =~ /^(\w+)\s+(\w+)/ ) {
    $symbols{$1} = "0x$2";
  }
}
close( SYM_FILE_HANDLE );

# See comment above
#
my %addresses_written=();

foreach my $lis_filename (@ARGV) {

  open( LIS_FILE_HANDLE, $lis_filename ) or die("No such input file \"$lis_filename\"\n");

  my $in_function           = undef;
  my $lis_file_offset       = "0000";
  my $offset_at_last_symbol = "0000";
  my $last_symbol_address   = "0000";
  my $last_mc_length        = 0;
  my $current_section       = "IGNORE";
  my $rest_of_line;

  my $skip_block = 0;
  while( my $line = <LIS_FILE_HANDLE> ) {
    chomp($line);
    $rest_of_line = $line;

    # Skip over the lis file IF 0/ENDIF blocks
    #
    if( $line =~ /^\s*\d+\s+IF\s+0/ ) {
      $skip_block = 1;
      print ">>> Processing OFF\n";
    }
    elsif( $skip_block && $line =~ /^\s*\d+\s+ENDIF/ ) {
      $skip_block = 0;
      print ">>> Processing ON\n";
    }

    # A new module (i.e. a new LIS file) means reset everything. All these
    # except the machine code length are stored in hex, which is how they
    # come out of the LIS file.
    #
    elsif( $line =~ /^\s*1\s+MODULE/ ) {
      $lis_file_offset       = "0000";
      $offset_at_last_symbol = "0000";
      $last_symbol_address   = "0000";
      $last_mc_length        = 0;
      print ">>> Start of module, reset all\n";
    }

    # Don't try to tag things which aren't code, stuff gets confused
    #
    elsif( $line =~ /^\s+\d+\s+SECTION\s(\w+)/ ) {
      $current_section = $1;
      print ">>> Inside section $1\n";
    }

    # Right, must be code then
    #
    else {

      # Only interested in what's in the code sections
      #
      next if( $current_section !~ /^code_/ );

      if( ! $skip_block ) {

	# Most lines start with, for example:
	#
	#    754  0000  dde5              	push	ix
	#
	# The first value is a line number in the *.lis file. The second
	# value is a hex number which indicates the hex offset from the
	# start of the list file. Then there's the machine code values.
	# The rest of the line is the assembler or comment.
	#
	# Function declaration lines are:
	#
	#    724                                _main:
	#
	# Caution, comment lines are:
	#
	#    760                                ;main.c:55: while(1)
	#
	# Caution, "defc", for define constant, is a hex string :)
	#
	if( $line =~ /^\s*\d+\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)(\s+.*)$/ and
	    $line !~ /DEFC.*=/i and
	    $line !~ /\sDEFS\s/ ) {

	  # This line has a single line of assembly language. This is the
	  # most common case. Pick up the offset into the memory map
	  # this ASM instruction is at.

	  # The line might also be a DEFB line, defining a single byte
	  # of storage, like this:
	  #
	  #  831  00c5  01                      DEFB +0x01
	  #
	  # which matches the regex and works correctly. Same for DEFM.

	  # Note the offset of this line from the start of the list file
	  #
	  $lis_file_offset = $1;
	  
	  # Number of bytes in the machine code instruction
	  #
	  $last_mc_length = length( $2 ) / 2;
	  # print ">>> Offset into LIS file is now $lis_file_offset, this machine code is $last_mc_length bytes ($2)\n";

	  # Rest of line
	  #
	  $rest_of_line = $3;

	}
	elsif( $line =~ /\s*\d+\s+DEFS\s+(\d+)\+/ ) {

	  # Line defines some storage at this memory address, like this:
	  #
	  #  0000                DEFS 2
	  #
	  # That format of line doesn't carry the offset, so just
	  # add the size to the current offset

	  $lis_file_offset = sprintf( "%04x", hex( $lis_file_offset ) + $1 );

	  # Not sure about this; it seems to work with all the files I've tried
	  #
	  $last_mc_length  = 0;

	}
	elsif( $line =~ /^\s*\d+\s+(_\w+):\s*$/ ) {

	  # This line is the label at the top of a new function. The address
	  # of this point - where the function is in the Spectrum's
	  # memory - comes from the symbols file. This is the reference
	  # address for the instructions which follow.

	  # If this is a new function it will be in the symbols file.
	  #
	  my $symbol = $1;

	  # Enable this with a troublesome symbol name to get a break on it
	  #
	  #if( $symbol eq "_select_controls" )
	  #{
	  #    print "breakpoint\n";
	  #}	    

	  if( exists( $symbols{$symbol} ) ) {

	    # A symbol appears in the LIS file like:
	    #
	    # ...
	    #   688  00d3  c9                	ret
	    #   689                          	SECTION code_compiler
	    #   690                          ;virion.c:79: void random_reappear_virion( VIRION *v )
	    #   691                          ;	---------------------------------
	    #   692                          ; Function random_reappear_virion
	    #   693                          ; ---------------------------------
	    #   694                          _random_reappear_virion:
	    #   695  00d4  dde5              	push	ix
	    # ...
	    #
	    # Bit of an issue here though. The $lis_file_offset value will be the last
	    # one seen, which here will be for the preceding "ret", so 00d3. But the
	    # first instruction of the new function, the "push ix", is at 00d4. So I
	    # need to work out how far in advance of the last known offset the first
	    # instruction of the routine is. The compiler used to provide the offset
	    # into the LIS file in the label line for each function, but not any
	    # more. It's an awkward look-ahead type thing, so I keep a note of the
	    # length of the previous machine instruction, which in this case is the
	    # "ret". That's the "c9", which is one byte. Adding that value (1) to the known
	    # lis_file_offset (0x00d3+1 in this case) gives the offset to the first
	    # instruction of the new routine (0x00d4). Feels a bit hacky, but it works.

	    $last_symbol_address = $symbols{$symbol};
	    $offset_at_last_symbol = sprintf( "%04x", hex( $lis_file_offset ) + $last_mc_length );
	    
	    # At this point I know the location in Spectrum memory of that
	    # symbol, and the location of the symbol's code from the start
	    # of the list file.

	    # print ">>> Symbol \"$symbol\" found at $symbols{$symbol} \n";
	  }
	  else {

	    # Symbol defined in the *.lis file but it's not in the symbols
	    # file. Code like this does this:
	    #
	    #  print_str(12,10,"Down? ");
	    #
	    # It produces:
	    #
	    #   1100                          ___str_10:
	    #   1101  0089  006f776e1308      	DEFM "Down? "
	    #   1102  008f  00                	DEFB 0x00
	    #
	    # So a label that's static to the source file and not in the
	    # symbols table. Safe to ignore unless something weird is
	    # happening.
	    #
	    # print "???? -- ".$symbol."\n";
	  }
	}	  
      }
    }

    # So for each line I can subtract the line's location in the list file
    # from the location in the list file of the last symbol - the function we're
    # parsing though. That gives the offset in bytes of the current line's
    # instruction from the start of the defintion of the function the symbol
    # refers to. Add that offset to where the function is in Spectrum memory
    # to find the address of the current line's m/c code in the Spectrum
    # memory.
    #
    my $address = hex($lis_file_offset) - hex($offset_at_last_symbol) + hex($last_symbol_address);
    if( !exists( $addresses_written{$address} ) )
    {
      printf("0x%04X ++ %s\n", $address&0xFFFF, $rest_of_line);
      $addresses_written{$address} = 1;
    }
    else
    {
      printf("0x     ++ %s\n", $rest_of_line);
    }
 
  }

  close( LIS_FILE_HANDLE );
}
