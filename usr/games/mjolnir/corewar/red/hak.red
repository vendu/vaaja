; redcode-88
; hak
; Tuomo 'vendu' Venäläinen
; bomb with DAT and SPL 0
; ...
; spl   0
; dat   #666
; spl   0
; dat   #666
; <program>
; dat   #666
; spl   0
; dat   #666
; spl   0
; ...
; begin
mov     11,     @-1     ; backward copy DAT                     ; 0
mov     10,     @12     ; forward copy DAT                      ; 1
mov     8,      @-4     ; backward copy SPL 0                   ; 2
mov     7,      @9      ; forward copy SPL 0                    ; 3
sub     2,      @-4     ; adjust backward DAT-copy offset       ; 4
add     2,      @-4     ; adjust forward DAT-copy offset        ; 5
sub     2,      @-4     ; adjust forward SPL-copy offset        ; 6
add     2,      @-4     ; adjust backward SPL-copy offset       ; 7
spl     -8              ; spawn at first MOV                    ; 8
jmp     -9              ; jump back to first MOV                ; 9
spl     #0              ; SPL to be copied                      ; 10
dat     #666            ; DAT to be copied                      ; 11
