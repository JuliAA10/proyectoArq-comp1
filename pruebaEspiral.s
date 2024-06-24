.text
.global ElEspiralASMB
.global QUIT
.global DELAY
.global DELAY_4
.global DisplayBinary
.global LedOutput
.global Delay
.global Clear

ElEspiralASMB:
    PUSH {R0,R4,R5,R6,R7,R8,R9,R10,R11,LR}
    BL Clear
    MOV R4, #0             // Init DISPLAY with 0
    LDR R11, =DELAY_4      // DELAY_4(Address)
    LDR R6, [R11]          // DELAY_4(Value)
    LDR R9, =DELAY         // DELAY(Address)
    STR R6, [R9]           // DELAY = DELAY_4
    LDR R10, =QUIT         // QUIT(Address)
    LDR R8, =table

while_loop:
    MOV R5, #0             // Init i = 0, for loop
for_loop:
    LDR R7, [R10]          // QUIT(Value)
    CMP R7, #1
    BEQ break_loop

    MOV R0, R4
    MOV R1, #4
    BL DisplayBinary

    MOV R0, R4
    BL LedOutput

    LDRB R4, [R8, R5]      // DISPLAY = table[i]

    LDR R6, [R9]           // DELAY(Value)
    MOV R0, R6
    BL Delay

    ADD R5, R5, #1
    CMP R5, #16
    BLT for_loop

    LDR R7, [R10]          // QUIT(Value)
    CMP R7, #0
    BEQ while_loop

break_loop:
    STR R6, [R11]
    MOV R0, #0
    STR R0, [R10]
    POP {R0,R4,R5,R6,R7,R8,R9,R10,R11,PC}

.data

table: 
    .byte 0x80
    .byte 0x81
    .byte 0xC1
    .byte 0xC3
    .byte 0xE3
    .byte 0xE7
    .byte 0xF7
    .byte 0xFF
    .byte 0xEF
    .byte 0xE7
    .byte 0xC7
    .byte 0xC3
    .byte 0x83
    .byte 0x81
    .byte 0x1
    .byte 0x0

.end
