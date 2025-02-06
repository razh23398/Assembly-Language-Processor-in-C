; file ps.as
.entry LIST 
.extern W 
.define RAZ = 2 
mcr raz
razi
mov r3, 5
endmcr
mcr RAZ
razi
endmcr
raz:	mov r3, LIST[sz]
LOOP: 	jmp W 
	prn #-5
	mov STR[5], STR[2] 
	sub r1, r4
	cmp K, #sz
	bne W
L1: 	inc L3 
.entry LOOP
	bne LOOP 
END: hlt
.define len = 4
STR: .string "abcdef" 
LIST: .data 6, -9, len 
K: .data 22 
.extern L3