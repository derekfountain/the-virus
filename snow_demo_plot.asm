;; This came from Andy Dansby's project here:
;; https://github.com/andydansby/pixel_racing_LUT
;; I don't know its history or what licence it
;; should be shown as under. Hopefully GPL compliant.
;; Thanks, Andy!

SECTION code_user
EXTERN _snow_tab

PUBLIC _snow_plot
PUBLIC _snow_unplot

_snow_plot:
    pop af		                  ; Callee convention, this is the return address
    pop de		                  ; Args are passed as 2 unsigned chars, x,y. Y is in D, X is in E
    push af	                   	; Push the return address back. Stack is now ready for the return.

  	ld h, _snow_tab / 256       ; table must be 256-byte aligned
    ld l,d                      ; coord Y from D, HL now has table address

    ld b,(hl)
    inc h
    ld a,(hl)
    ld l,e
    inc h
    or (hl)
    inc h
    ld c,a

; BC contains screen address
; HL contains table address

    ld a,(bc)
    or (hl)
    ld (bc),a
    ret


; Here's the unplot. It's a duplication for the most part, but my
; original used an extra parameter, a comparison and a jump. This
; is faster.
_snow_unplot:
    pop af                  		; Callee convention, this is the return address
    pop de	                  	; Args are passed as 2 unsigned chars, x,y. Y is in D, X is in E
    push af		                  ; Push the return address back. Stack is now ready for the return.

  	ld h, _snow_tab / 256       ; table must be 256-byte aligned
    ld l,d                      ; coord Y from D, HL now has table address

    ld b,(hl)
    inc h
    ld a,(hl)
    ld l,e
    inc h
    or (hl)
    inc h
    ld c,a

; BC contains screen address
; HL contains table address

    ld a,(hl)                   ; pick up "plot" value from table
    cpl                         ; invert
    ld d,a                      ; stash somewhere

    ld a,(bc)                   ; pick up screen value
    and d                       ; reset the bit to unplot
    ld (bc),a                   ; update screen
    ret
