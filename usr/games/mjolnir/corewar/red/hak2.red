; redcode-88
; hak
; Tuomo 'vendu' Venäläinen
; bomb with DAT and SPL 0
dat     #666            ; DAT to be copied                      ; 0
; begin
mov     10,      @-2    ; backward copy SPL                     ; 1
mov     -2,     @-3     ; backward copy DAT                     ; 2
mov     8,      @9      ; forward copy SPL                      ; 3
mov     -4,     @9      ; forward copy DAT                      ; 4
sub     2,      @-4     ; adjust backward SPL-copy offset       ; 5
add     2,      @-4     ; adjust forward SPL-copy offset        ; 6
sub     2,      @-4     ; adjust backward DAT-copy offset       ; 7
add     2,      @-4     ; adjust forward DAT-copy offset        ; 8
spl     -8              ; spawn at first MOV                    ; 9
jmp     -9                                                      ; 10
spl     0               ; SPL to be copied                      ; 11
