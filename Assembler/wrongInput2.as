; file ps.as
;.entry LENGTH
.extern W
MAIN: mov r3
LOOP: jmp L1
 prn #-541231
 bne W 
 sub r1, r4
 bne L3

 .data 5  , -25 ,  -5, 14, 1

.data , 5,   12 ,  -110

.data 5 ,, 22 , 45

.data 34  , -100 ,        
                                                                                                                                                                                                                                                                                                                                    


L1: inc K
.entry LOOP
 bne LOOP
END: stop
STR:.string "abc1231123def"
r12 : .data 6               ,-9             ,15,    +123    ,   +12    ,-12
                                                                                                                                                                                                                                                                                                                                                                                                             

K: .data 22
.extern L3 
