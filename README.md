# proyectoArq-comp1

# Año: 2024

# Integrantes:

- Alvarez Aguirre, Julián
- Arrieta, Iván
- Diaz Otañez, Hugo

## Install Libs:

sudo apt install libncurses5-dev
sudo apt install libcurl4-openssl-dev

## Compile:

gcc main.c -o main -lpthread -lncurses -lcurl

## Raspberry:

gcc espiral.s rebote.s choque.s autofantastico.s main.c -o main -lpthread -lncurses -lcurl

gcc -o main.o main.c ass.o (todos los assemblys)
