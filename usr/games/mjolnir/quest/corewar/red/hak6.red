; redcode
; hak6
; Tuomo 'vendu' Venäläinen
begin   mov     11      7929    ; copy DAT
mov     11      7956    ; copy SPL
mov     9,      7983    ; copy DAT
mov     9,      8031    ; copy SPL
sub     #31,    -4      ; subtract from 1st copy DAT offset
add     #31,    -4      ; add to 1st copy SPL offset
spl     -6
sub     #63,    -7
add     #63,    -7
jmp     -9
dat     0,      0
spl     0
end     begin
