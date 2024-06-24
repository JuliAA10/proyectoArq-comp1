.text
.global ElReboteASMB
.global QUIT
.global DELAY
.global DELAY_3
.global DisplayBinary
.global LedOutput
.global Delay
.global Clear

ElReboteASMB:
    PUSH {R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,LR}
    BL Clear
    LDR R11, =DELAY_3 // DELAY_3(Address)
    LDR R6, [R11] // DELAY_3(Value)
    LDR R9, =DELAY // DELAY(Address)
    STR R6, [R9] // DELAY = DELAY_3
    LDR R10, =QUIT // QUIT(Address)

    while:
        MOV R8, #7 // Init r = 0, for loop
        MOV R4, #0x80 // Init DISPLAY with 0
        for:
            for_outer:
                LDR R7, [R10] // QUIT(Value)
                CMP R7, #1
                BEQ break

                MOV R0, R4
                MOV R1, #3
                BL DisplayBinary

                MOV R0, R4
                BL LedOutput

                LSR R4, R4, #1
                 
                LDR R6, [R9] // DELAY(Value)
                MOV R0, R6
                BL Delay

                CMP R4, #0b1
                BNE for_outer

            MOV R5, #0 // Init j = 0, for loop_inner
            for_inner:
                LDR R7, [R10] // QUIT(Value)
                CMP R7, #1
                BEQ break

                MOV R0, R4
                MOV R1, #3
                BL DisplayBinary

                MOV R0, R4
                BL LedOutput

                LSL R4, R4, #1

                LDR R6, [R9] // DELAY(Value)
                MOV R0, R6
                BL Delay

                ADD R5, R5, #1
                SUB R8, R8, #1
                CMP R5, R8
                ADD R8, R8, #1
                BLT for_inner

            SUB R8, R8, #1
            CMP R8, #0
            BGT for

        LDR R7, [R10] // QUIT(Value)
        CMP R7, #1
        BEQ break

        MOV R0, R4
        MOV R1, #3
        BL DisplayBinary

        MOV R0, R4
        BL LedOutput

        LDR R6, [R9] // DELAY(Value)
        MOV R0, R6
        BL Delay

        LDR R7, [R10] // QUIT(Value)
        CMP R7, #0
        BEQ while

    break:
    STR R6, [R11]
    MOV R0, #0
    STR R0, [R10]
    POP {R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,PC}

.data
.end