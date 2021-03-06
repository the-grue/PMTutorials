;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; PMode tutorials in C and Asm                          ;;
;; Copyright (C) 2000 Alexei A. Frounze                  ;;
;; The programs and sources come under the GPL           ;;
;; (GNU General Public License), for more information    ;;
;; read the file gnu-gpl.txt (originally named COPYING). ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GLOBAL  _read_msw, _read_cr0, _write_cr0

SEGMENT _TEXT PUBLIC CLASS=CODE USE16

_read_msw:
        smsw    ax
        retn

_read_cr0:
        mov     eax, cr0        ; read CR0 to eax
        mov     edx, eax
        shr     edx, 16         ; dx:ax = CR0 (return value)
        retn

_write_cr0:
        push    bp
        mov     bp, sp
        mov     eax, [ss:bp+4]  ; eax = 32-bit parameter
        mov     cr0, eax
        pop     bp
        retn

SEGMENT _DATA PUBLIC CLASS=DATA
