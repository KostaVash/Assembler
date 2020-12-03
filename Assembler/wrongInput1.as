; file ps.as
;.entry LENGTH
.extern W
MAIN: mov r3
LOOP: jmp L1(#-1,r6)
 prn #-5
 bne W(r4,r5)
add r3,   #4
cmp #050, #-212
He45: jsr r3
prn NUMBER
 sub r1, r4
 bne L3
string: mov 5 , r8
stop r5
add Main#5
L1: inc K
.entry LOOP
 bne LOOP(K,W)
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15,1234,+1234,0,0,1234,12,12,12,14,-10000,1231,124123   ,123123,
   
K: .data 22
.extern L3 
