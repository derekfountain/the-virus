;; Shake-the-virus, a ZX Spectrum game
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

SECTION code_user

PUBLIC _rtunes_pixel
_rtunes_pixel:

    pop af		; Callee convention, this is the return address
    pop de		; Args are passed as 2 unsigned chars, x,y. Y is in D, X is in E
    pop bc    ; C=1 for draw, 0 for clear, caller puts single byte on stack so B not used
;    dec sp    ; adjust for single byte
    push af		; Push the return address back. Stack is now ready for the return.

    LD A,D
    AND A
    RRA
    SCF ; Set Carry Flag
    RRA
    AND A
    RRA
    XOR d
    AND %11111000   ;248
    XOR d
    LD H,A          ;LD D,A
    LD A,e
    RLCA
    RLCA
    RLCA
    XOR d
    AND %11000111   ;199 / $C7
    XOR d
    RLCA
    RLCA
    LD L,A

    LD A,e
    AND 7

    LD d, A

    LD DE, rtunes_bits
    ADD A,E
    LD E,A        ; Plot pattern address in DE

    EX DE,HL      ; HL now points to plot pattern
    LD B,(HL)     ; plot pattern is in B
    EX DE,HL      ; HL back pointing at screen address

    ;output to screen, are we plotting or unplotting?
    ld a,c
    cp 1
    ld a,b        ; Plot pattern in A
    jr z, plot_b

unplot_b:
    cpl           ; invert bit pattern and AND into screen byte
    and (hl)
    ld (hl),a
    ret

plot_b:
    or (hl)       ; OR bit pattern into screen byte
    ld (hl),a

    ret



rtunes_bits: defb 128,64,32,16,8,4,2,1
