;; The Virus, a ZX Spectrum game
;; Copyright (C) 2022 Derek Fountain
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

; Pixel plotting table goes in the top 1K
SECTION snow_table
org 0xfc00

; The font is 770 bytes and goes below the snow table
SECTION font
org 0xf8fe

; The pixel plotting code is only about 38 bytes, so there's
; plenty of room to drop that in here.
SECTION snow_code
org 0xf8d7





; The interrupt vector table is 257 bytes at 0xd1d1. That's
; put in place by code (int.c). The 3 byte jump instruction is
; at 0xd1d1, 0xd1d2 and 0xd1d3. The stack is underneath the
; vector table so there's a useful block of memory above the
; jump instruction.
SECTION extra_room
org 0xd1d4
