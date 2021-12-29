# Shake-the-virus, a ZX Spectrum game
# Copyright (C) 2022 Derek Fountain
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

CC=zcc
AS=zcc
APPMAKE=z88dk-appmake
TARGET=+zx
VERBOSITY=-vn
CRT=4

PRAGMA_FILE=zpragma.inc

#C_OPT_FLAGS=-SO2 --max-allocs-per-node200000 -DNDEBUG --std-c99 --list
C_OPT_FLAGS=--c-code-in-asm --std-c99 --list

CFLAGS=$(TARGET) $(VERBOSITY) -c $(C_OPT_FLAGS) -preserve -compiler sdcc -clib=sdcc_iy -pragma-include:$(PRAGMA_FILE)
LDFLAGS=$(TARGET) $(VERBOSITY) -m -clib=sdcc_iy -pragma-include:$(PRAGMA_FILE)
ASFLAGS=$(TARGET) $(VERBOSITY) -c

CPP_FLAGS=$(TARGET) $(VERBOSITY) -c -compiler sdcc -clib=sdcc_iy -pragma-include:$(PRAGMA_FILE) -E

SYMBOLS_GENERATOR=./generate_symbols.pl
MAP=stv.map

TAGGABLE_SRC_GENERATOR=./generate_taggable_src.pl
TAGGABLE_SRC=stv.taggable_src

BE_MAP_EXTRACTOR=./zxspectrum_be/extract_enums.tcl
BE_ENUMS=./stv_enums.inc
BE_ENUMS_LIST=./enums_list.lis
BE_STRUCT_EXTRACTOR=./zxspectrum_be/extract_structs.tcl
BE_STRUCTS=./stv_structs.inc
BE_STATICS_EXTRACTOR=./zxspectrum_be/extract_statics.tcl
BE_STATICS=./stv_statics.inc

TAGS_GENERATOR=etags
TAGS=TAGS

MEM_FREE=./how_much_memory_left.sh

EXEC=stv.tap
EXEC_OUTPUT=stv
SYM_OUTPUT=stv.sym

OBJECTS = main.o \
	  player.o \
	  controls.o \
	  int.o \
	  level.o \
	  levels.o \
	  swarm.o \
          virion.o \
	  game_over.o

# Objects built from C files (as opposed to ASMs)
C_OBJECTS = main.o \
            player.o \
	    controls.o \
	    int.o \
	    level.o \
            levels.o \
	    swarm.o \
            virion.o \
	    game_over.o

# A .cpre is the output of the C preprocessor
PREPROCESSED = $(C_OBJECTS:.o=.cpre)

# For now, if any header changes, recompile the lot.
HEADERS = main.h \
	  player.h \
	  level.h \
	  levels.h \
	  controls.h \
	  int.h \
	  swarm.h \
	  virion.h \
	  game_over.h

# Run the preprocessor on *.c files to get *.cpre files
%.cpre: %.c $(PRAGMA_FILE) $(HEADERS)
	$(CC) $(CPP_FLAGS) -o $@ $<

%.o: %.c $(PRAGMA_FILE) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<


all : clean_tmp $(EXEC) $(SYM_OUTPUT) $(TAGGABLE_SRC) $(BE_ENUMS) $(BE_STRUCTS) $(BE_STATICS) $(TAGS) report

# Rule to build the executable.
$(EXEC) : $(OBJECTS)
	$(CC) $(LDFLAGS) -startup=$(CRT) $(OBJECTS) -o $(EXEC_OUTPUT)
	$(APPMAKE) +glue -b stv --filler 0xDF --clean
	$(APPMAKE) +zx -b stv__.bin --org 32768 --blockname stv -o $(EXEC)

# Build the symbols table after the executable
$(SYM_OUTPUT): $(EXEC)
	$(SYMBOLS_GENERATOR) < $(MAP) > $(SYM_OUTPUT)

# Tagged source needs symbol table
$(TAGGABLE_SRC): $(SYM_OUTPUT)
	$(TAGGABLE_SRC_GENERATOR) $(SYM_OUTPUT) *.c.lis > $(TAGGABLE_SRC)

# BE enums extractor runs after a successful build
$(BE_ENUMS): $(EXEC)
	$(BE_MAP_EXTRACTOR) --enum-list $(BE_ENUMS_LIST) *.h *.c > $(BE_ENUMS)

# BE structs extractor runs after a successful build, plus it requires the enum list
$(BE_STRUCTS): $(EXEC) $(BE_ENUMS)
	$(BE_STRUCT_EXTRACTOR)  --enum-list $(BE_ENUMS_LIST) $(HEADERS) *.c > $(BE_STRUCTS)

# Statics processing needs preprocessor output plus the symbols table
$(BE_STATICS): $(PREPROCESSED) $(SYM_OUTPUT)
	$(BE_STATICS_EXTRACTOR) --symbols-file $(SYM_OUTPUT) *.cpre > $(BE_STATICS)

# TAGS file generated each time the binary is rebuilt for any reason
$(TAGS): $(EXEC)
	$(TAGS_GENERATOR) *.h *.c

# After everything, report the exec size
.PHONY: report
report: $(SYM_OUTPUT) $(TAGGABLE_SRC) $(BE_ENUMS) $(BE_STRUCTS) $(BE_STATICS) $(TAGS)
	$(MEM_FREE)

.PHONY: clean_tmp
clean_tmp:
	rm -f /tmp/tmpXX* 

.PHONY: clean
clean :
	rm -f *.tap *.cpre *.bin *.map *.sym *.o *.lis stv*.inc TAGS *~ $(BE_ENUMS) $(TAGGABLE_SRC)
