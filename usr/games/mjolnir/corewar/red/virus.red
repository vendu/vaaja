; redcode-88
; virus
; David Moore's CWS'88 example
; bomber
spl     1
spl     1
mov     -1
spl     0,      <2
mov     0,      <1
mov     4,      978
add     #2201,  @-2
mov     -4,     @-2
jmp     @-3
djn     -2,     #6970
