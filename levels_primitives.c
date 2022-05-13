/*
 * The Virus, a ZX Spectrum game.
 * Copyright (C) 2022 Derek Fountain
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdint.h>
#include <arch/zx.h>

void draw_cells( uint8_t cells[][2], uint8_t colour )
{
  uint8_t i=0;

  while( cells[i][0] != 0xFF )
  {
    *(zx_cxy2aaddr(cells[i][0],cells[i][1])) = colour;

    i++;
  }
}

void _2x2( uint8_t x, uint8_t y, uint8_t colour )
{
  uint8_t *att_addr = zx_cxy2aaddr(x, y);
  *(att_addr) = colour;
  att_addr++;
  *(att_addr) = colour;
  att_addr+=31;
  *(att_addr) = colour;
  att_addr++;
  *(att_addr) = colour;
}

void _5x1( uint8_t x, uint8_t y, uint8_t colour )
{
  uint8_t *att_addr = zx_cxy2aaddr(x, y);

  *(att_addr++) = colour;
  *(att_addr++) = colour;
  *(att_addr++) = colour;
  *(att_addr++) = colour;
  *(att_addr++) = colour;
}

void draw_box( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t colour )
{
  uint8_t *att_addr1 = zx_cxy2aaddr(x1, y1);
  uint8_t *att_addr2 = zx_cxy2aaddr(x1, y2);

  uint8_t i;
  for( i=x1; i<=x2; i++ )
  {
    *(att_addr1++) = colour;
    *(att_addr2++) = colour;
  }

  att_addr1 = zx_cxy2aaddr(x1, y1);
  att_addr2 = zx_cxy2aaddr(x2, y1);
  for( i=y1; i<=y2; i++ )
  {
    *att_addr1 = colour;
    att_addr1 += 32;
    *att_addr2 = colour;
    att_addr2 += 32;
  }
}

void draw_h_line( uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour )
{
  uint8_t *att_addr = zx_cxy2aaddr(x1, y);

  uint8_t i;
  for( i=x1; i<=x2; i++ )
  {
    *(att_addr++) = colour;
  }
}

void swap_cells_colours( uint8_t src, uint8_t dest )
{
  register uint8_t *addr = (uint8_t*)0x5ADF;
  while( addr >= (uint8_t*)0x5800 )
  {
    if( *addr == src )
      *addr = dest;
    else if( *addr == dest )
      *addr = src;

    addr--;
  }
}
