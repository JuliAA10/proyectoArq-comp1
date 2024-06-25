//Definiciones Globales
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
    PUSH {R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,LR} //Guarda los registros especificos en la pila
    BL Clear               // Limpia pantalla
    LDR R11, =DELAY_3      // DELAY_3(Address)
    LDR R6, [R11]          // DELAY_3(Value)
    LDR R9, =DELAY         // DELAY(Address)
    STR R6, [R9]           // DELAY = DELAY_3
    LDR R10, =QUIT         // QUIT(Address)

while_loop:
    MOV R8, #7             // Init r = 7, for loop controla cant iteraciones
    MOV R4, #0x80          // Init DISPLAY with 0x80 efecto rebote
outer_for_loop:            // Desplazar a la derecha el valor en R4, mostrarlo y aplicar un retardo
    LDR R7, [R10]          // QUIT(Value)
    CMP R7, #1
    BEQ break_loop         // Si QUIT=1 sale del bucle

    MOV R0, R4
    MOV R1, #3
    BL DisplayBinary

    MOV R0, R4
    BL LedOutput

    LSR R4, R4, #1

    LDR R6, [R9]           // DELAY(Value)
    MOV R0, R6
    BL Delay

    CMP R4, #0b1
    BNE outer_for_loop

    MOV R5, #0             // Init j = 0, inner_for_loop
inner_for_loop:            // Efecto de rebote hacia la izquierda se muestran, se envían los datos a la salida y también se aplica un retardo
    LDR R7, [R10]          // QUIT(Value)
    CMP R7, #1
    BEQ break_loop         // Si QUIT=1 sale del bucle

    MOV R0, R4
    MOV R1, #3
    BL DisplayBinary

    MOV R0, R4
    BL LedOutput

    LSL R4, R4, #1

    LDR R6, [R9]           // DELAY(Value)
    MOV R0, R6
    BL Delay

    ADD R5, R5, #1
    SUB R8, R8, #1
    CMP R5, R8
    ADD R8, R8, #1
    BLT inner_for_loop

    SUB R8, R8, #1
    CMP R8, #0
    BGT outer_for_loop

    LDR R7, [R10]          // QUIT(Value)
    CMP R7, #1
    BEQ break_loop         // Si QUIT=1 sale del bucle

    MOV R0, R4
    MOV R1, #3
    BL DisplayBinary

    MOV R0, R4
    BL LedOutput

    LDR R6, [R9]           // DELAY(Value)
    MOV R0, R6
    BL Delay

    LDR R7, [R10]          // QUIT(Value)
    CMP R7, #0
    BEQ while_loop

break_loop:
    STR R6, [R11]          // Guarda el valor de DELAY de vuelta en DELAY_3
    MOV R0, #0
    STR R0, [R10]          //Establece QUIT=0
    POP {R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,PC} //Desapila los registros y los restaura a los valores predeterminados

.data
.end
