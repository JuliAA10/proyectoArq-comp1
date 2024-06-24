#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "EasyPIO.h"

#define QUIT_KEY 'q'
#define DELAY_INTERVAL 250
#define DEFAULT_DELAY 1000


const unsigned char led[8] = {7, 8, 25, 24, 23, 18, 15, 14};

int main(){
    pioInit();

    for (int i = 0; i < 8; i++){
        pinMode(led[i], OUTPUT);
    }

    initscr();              // Inicializar la pantalla de ncurses
    keypad(stdscr, TRUE);   // Habilitar la entrada de teclado especial
    nodelay(stdscr, TRUE);  // Configurar getch() para no bloquear
    noecho();               // No mostrar los caracteres ingresados en pantalla

    App();

    endwin();

}

void App(){
    unsigned char option[1];
    Clear();

    int i = 0, a = 0;

    for (i; i < 3; i++){
        Delay(2000);
        Clear();
        if (Login())
            break;
    }

    if (i == 3)
        exit(0);

    do {
        Delay(2000);
        DisplayBinary(0, 0);
        Clear();
        printf("\033[?25h");

        printf("----- S E C U E N C I A S  D E  L U C E S -----\n\r");
        printf("1. Auto Fantastico\n\r");
        printf("2. El Choque\n\r");
        printf("3. El Rebote\n\r");
        printf("4. El Espiral\n\r");
        printf("5. El Caos\n\r");
        printf("0. Salir\n\r");
        printf("-------------------------------------------------\n\r");
        printf("\n\rSeleccione una opcion: ");

        scanf("%s1", &option[0]);

        if (option[1] != '\0') {
        option[0] = 'i';
        }

        printf("\033[?25l");

    switch (option[0]) {
        pthread_t threads[2];
        

        case '1':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, AutoFantastico, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            break;

        case '2':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, ElChoque, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            break;

        case '3':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, ElRebote, NULL);
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            break;

        case '4':
            pthread_create(&threads[0], NULL, keyListener, NULL);
            pthread_create(&threads[1], NULL, ElEspiral, NULL);
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
            printf("\n\rSaliendo del Programa..\n\r"):
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