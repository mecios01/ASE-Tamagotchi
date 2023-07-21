                AREA    |.util_text|, CODE, READONLY


; Count leading zeros

_RIT_mask  PROC
                EXPORT  _RIT_mask
                
                STMFD sp!,{r4-r8,r10-r11,lr}
				CLZ R0,R0
				MOV R1,#0xFFFFFFFF
				LSR R0,R1,R0
				EOR R0,R0,R1
				LDMFD sp!,{r4-r8,r10-r11,pc}

                BX      R0
                ENDP
		END
