/*Cola de arreglos de caracteres.*/

#ifndef COLA_H
#define COLA_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Nodo {
    char* dato;
    struct Nodo* sig;
} nodo_t;

typedef struct Cola {
    nodo_t *pri, *ult;
} cola_t;

nodo_t* crear_nodo(char* str);

cola_t* crear_cola();

void encolar(cola_t* cola, char* str);

char* desencolar(cola_t* cola);

void free_cola(cola_t* cola);


#endif
