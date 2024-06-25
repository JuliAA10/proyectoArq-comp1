#include <stdlib.h>
#include <pthread.h> //biblioteca que se encarga de dividir las tareas de los subprocesos más facilmente
#include <stdio.h>
#include "EasyPIO.h"
#include <ncurses.h>
#include <string.h>

#define PASSWORD "12345"
#define QUIT_KEY 'q'
#define DELAY_INTERVAL 250
#define DEFAULT_DELAY 1000

unsigned int QUIT;
size_t DELAY;
size_t DELAY_1 = DEFAULT_DELAY;
size_t DELAY_2 = DEFAULT_DELAY;
size_t DELAY_3 = DEFAULT_DELAY;
size_t DELAY_4 = DEFAULT_DELAY;
size_t DELAY_5 = DEFAULT_DELAY;

pthread_mutex_t lock; //mutual exclusion, sincroniza el uso de un recurso compartido entre hilos. Así lo definimos

const unsigned char led[8] = {7, 8, 25, 24, 23, 18, 15, 14}; //definimos los leds

extern void ElEspiralASMB(); //assembly Esprial
extern void ElReboteASMB(); //assembly Rebote

void Delay(size_t a){ //crear un bucle de espera, permite controlar el tiempo de las secuencias de LEDs
    a = a * 100000; //para convertirlo en un valor mayor y poder controlarlo
    while (a--) //retardo
        ;
}

unsigned int Login(){
    char *right_password = PASSWORD;
    char input[6]; // Ajuste para 5 caracteres + terminador nulo
    int i = 0;
    char c;
    printf("Ingrese su password de 5 digitos: ");

    while (1) {
        c = getchar(); //lee un carácter desde la entrada estándar

        if (c == '\n') {
            input[i] = '\0';
            break;
        } else if (c == 127 || c == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            if (i < 5) {
                printf("*");
                input[i] = c;
                i++;
            }
        }
        if (i == 5) break;
    }

    input[i] = '\0';

    if (strcmp(input, right_password) == 0) { //comparar cadenas con strcmp
        printf("\n\rAcceso concedido\n\r");
        return 1;
    } else {
        printf("\n\rAcceso denegado\n\r");
        return 0;
    }
}

void MoveCursorToOrigin(){
    unsigned int i = 10;
    while (i--)
        printf("\033[F");
}

void Clear(){
    printf("\033[2J");
    MoveCursorToOrigin();
}

void DisplayBinary(unsigned char DISPLAY, unsigned int option){ //convierte los 1 en '*' y los 0 en '_'
    char display[8];
    int i = 0;
    for (unsigned int POINTER = 0x80; POINTER > 0; POINTER = POINTER >> 1){
        if (POINTER & DISPLAY) {
            display[i] = '*';
            i++;
        } else {
            display[i] = '_';
            i++;
        }
    }

    switch (option){
        case 1:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mAuto Fantastico\033[0m\n\r\n\r");
            break;
        case 2:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mEl Choque\033[0m\n\r\n\r");
            break;
        case 3:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mEl Rebote\033[0m\n\r\n\r");
            break;
        case 4:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mEl Espiral\033[0m\n\r\n\r");
            break;
        case 5:
            printf("\033[1;31mSECUENCIA:\033[0m \033[1;36mEl Caos\033[0m\n\r\n\r");
            break;
        case 0:
            break;
    }

    printf("%s", display);

    if (option) {
        printf("\n\r\n\r");
        printf("\033[1;30mPresione la tecla Q para salir\033[0m\n\r");
    }

    MoveCursorToOrigin();
}

void LedOutput(unsigned char DISPLAY) {
    int i = 0;
    for (unsigned int POINTER = 0x80; POINTER > 0; POINTER = POINTER >> 1){ //ciclo para controlar los LEDs
        if (POINTER & DISPLAY) {
            digitalWrite(led[i], 1); //se activa
            i++;
        } else {
            digitalWrite(led[i], 0); //no se activa
            i++;
        }
    }
}

void *keyListener(){
    while (!QUIT){
        int key = getch(); //leer una tecla
        pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
        if (key == QUIT_KEY)
            QUIT = 1; //establece QUIT a 1 si se presiona la tecla q
        else if (key == KEY_UP){
            if (DELAY - DELAY_INTERVAL != 0)
                DELAY -= DELAY_INTERVAL; //disminuye
        } else if (key == KEY_DOWN){
            DELAY += DELAY_INTERVAL; //aumenta
        }
        pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    }
    return NULL;
}

void *AutoFantastico() { //funcion para el autofantastico
    Clear();
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY = DELAY_1;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    while (!QUIT) {
        unsigned char DISPLAY = 0x80;
        for (int i = 0; i < 7; i++) {
            if (QUIT) break;
            DisplayBinary(DISPLAY, 1);
            LedOutput(DISPLAY);
            DISPLAY = DISPLAY >> 1;
            Delay(DELAY);
        }
        for (int i = 0; i < 7; i++) {
            if (QUIT) break;
            DisplayBinary(DISPLAY, 1);
            LedOutput(DISPLAY);
            DISPLAY = DISPLAY << 1;
            Delay(DELAY);
        }
    }
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY_1 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    return NULL;
}

void *ElChoque() { //funcion para el choque implementacion con tablas
    Clear();
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY = DELAY_2;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    unsigned int table[] = {0x42, 0x24, 0x18, 0x24, 0x42, 0x81};
    unsigned char DISPLAY = 0x81;
    while (!QUIT) {
        int table_size = sizeof(table) / sizeof(table[0]);
        for (int i = 0; i < table_size; i++) {
            if (QUIT) break;
            DisplayBinary(DISPLAY, 2);
            LedOutput(DISPLAY);
            DISPLAY = table[i];
            Delay(DELAY);
        }
    }
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY_2 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    return NULL;
}

void *ElReboteASMBP() { //funcion void que conecta la funcion del .s con esta para utilizarla más abajo
    ElReboteASMB();
    return NULL;
}

void *ElEspiralASMBP() { //funcion void que conecta la funcion del .s con esta para utilizarla más abajo
    ElEspiralASMB();
    return NULL;
}

void *ElRebote() { //funcion del rebote
    Clear();
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY = DELAY_3;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    while (!QUIT) {
        unsigned char DISPLAY = 0x80;
        for (int r = 7; r > 0; r--) {
            for (DISPLAY; DISPLAY != 0b1; DISPLAY = DISPLAY >> 1) {
                if (QUIT) break;
                DisplayBinary(DISPLAY, 3);
                LedOutput(DISPLAY);
                Delay(DELAY);
            }
            for (int j = 0; j < r - 1; j++) {
                if (QUIT) break;
                DisplayBinary(DISPLAY, 3);
                LedOutput(DISPLAY);
                DISPLAY = DISPLAY << 1;
                Delay(DELAY);
            }
        }
        if (QUIT) break;
        DisplayBinary(DISPLAY, 3);
        LedOutput(DISPLAY);
        Delay(DELAY);
    }
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY_3 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    return NULL;
}

void *ElEspiral() { //funcion del espiral implementado mediante tablas
    Clear();
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY = DELAY_4;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    unsigned int table[] = {0x80, 0x81, 0xC1, 0xC3, 0xE3, 0xE7, 0xF7, 0xFF, 0xEF, 0xE7, 0xC7, 0xC3, 0x83, 0x81, 0x1, 0x0};
    unsigned char DISPLAY = 0;
    while (!QUIT) {
        int table_size = sizeof(table) / sizeof(table[0]);
        for (int i = 0; i < table_size; i++) {
            if (QUIT) break;
            DisplayBinary(DISPLAY, 4);
            LedOutput(DISPLAY);
            DISPLAY = table[i];
            Delay(DELAY);
        }
    }
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY_4 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    return NULL;
}

void *ElCaos() { //ultima funcion por las dudas, del caos
    Clear();
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY = DELAY_5;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    unsigned char DISPLAY = 0;
    while (!QUIT) {
        unsigned char SUB_DISPLAY_1 = 0x80;
        unsigned char SUB_DISPLAY_2 = 0x01;
        for (int i = 0; i < 4; i++) {
            DISPLAY += SUB_DISPLAY_1;
            SUB_DISPLAY_1 = SUB_DISPLAY_1 >> 1;
            if (QUIT) break;
            DisplayBinary(DISPLAY, 5);
            LedOutput(DISPLAY);
            Delay(DELAY);
            DISPLAY += SUB_DISPLAY_2;
            SUB_DISPLAY_2 = SUB_DISPLAY_2 << 1;
            if (QUIT) break;
            DisplayBinary(DISPLAY, 5);
            LedOutput(DISPLAY);
            Delay(DELAY);
        }
        for (int i = 0; i < 4; i++) {
            DISPLAY -= SUB_DISPLAY_2;
            SUB_DISPLAY_2 = SUB_DISPLAY_2 >> 1;
            if (QUIT) break;
            DisplayBinary(DISPLAY, 5);
            LedOutput(DISPLAY);
            Delay(DELAY);
            DISPLAY -= SUB_DISPLAY_1;
            SUB_DISPLAY_1 = SUB_DISPLAY_1 << 1;
            if (QUIT) break;
            DisplayBinary(DISPLAY, 5);
            LedOutput(DISPLAY);
            Delay(DELAY);
        }
    }
    pthread_mutex_lock(&lock); //bloqueamos que otro hilo acceda a este recurso compartido
    DELAY_5 = DELAY;
    QUIT = 0;
    pthread_mutex_unlock(&lock); //desbloquea, por ende permite que otro hilo acceda a este recurso compartido
    return NULL;
}

void App(){
    unsigned char option[2]; // Ajuste para recibir un solo carácter
    Clear();

    int i = 0, a = 0;

    for (i; i < 3; i++){ //funcion para el login, si se intenta 3 veces, aborta
        Delay(2000);
        Clear();
        if (Login())
            break;
    }

    if (i == 3)
        exit(0);

    do {
        Delay(2000); //retardo 2s
        DisplayBinary(0, 0);
        Clear();
        printf("\033[?25h"); //muestra el cursor

        printf("------ S E C U E N C I A S  D E  L U C E S ------\n\r");
        printf("1. Auto Fantastico\n\r");
        printf("2. El Choque\n\r");
        printf("3. El Rebote\n\r");
        printf("4. El Espiral\n\r");
        printf("5. El Caos\n\r");
        printf("0. Salir\n\r");
        printf("-------------------------------------------------\n\r");
        printf("\n\rSeleccione una opcion: ");

        scanf(" %c", &option[0]);  //ignora espacios antes de leer el caracter, y va destinado a option[0]
        option[1] = '\0'; // Aseguramos que sea un string válido y la cadena termine despues del primer caracter

        printf("\033[?25l"); //oculta el cursor

        pthread_t threads[2]; //arreglo de 2 hilos

        switch (option[0]){
            case '1':
                pthread_create(&threads[0], NULL, keyListener, NULL); //se crean los hilos
                pthread_create(&threads[1], NULL, AutoFantastico, NULL); //se crean los hilos
                pthread_join(threads[0], NULL); //espera a que los hilos terminen
                pthread_join(threads[1], NULL); //espera a que los hilos terminen
                break;

            case '2':
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, ElChoque, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                break;

            case '3':
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, ElReboteASMBP, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                break;

            case '4':
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, ElEspiralASMBP, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                break;

            case '5':
                pthread_create(&threads[0], NULL, keyListener, NULL);
                pthread_create(&threads[1], NULL, ElCaos, NULL);
                pthread_join(threads[0], NULL);
                pthread_join(threads[1], NULL);
                break;

            case '0':
                printf("\n\rSaliendo del Programa..\n\r");
                Delay(2000);
                Clear();
                a = 1;
                break;

            default:
                printf("\n\rIngrese una opcion valida\n\r");
                break;
        }
    } while (a == 0);
}

int main(){
    pioInit();
    pthread_mutex_init(&lock, NULL); //inicializa el mutex

    for (int i = 0; i < 8; i++){
        pinMode(led[i], OUTPUT);
    }

    initscr();              // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE);   // Habilitar la entrada de teclado especial
    nodelay(stdscr, TRUE);  // Configurar getch() para no bloquear
    noecho();               // No mostrar los caracteres ingresados en pantalla

    App();

    endwin();
    pthread_mutex_destroy(&lock); //destruye el mutex creado anteriormente

    return 0;
}
