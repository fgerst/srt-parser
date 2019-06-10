/*Lista que almacena un subtitulo en cada nodo*/

#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>
#include "subs.h"

typedef struct NodoL {
    struct struct_subs *sub;
    struct NodoL *sig;
} nodol_t;

typedef nodol_t *lista_t;

void crear_lista(lista_t*);

int es_vacia(lista_t);

int buscar(lista_t, int);

void insertar_ordenado(lista_t*, struct struct_subs*);

void insertar(lista_t*, struct struct_subs*);

int eliminar(lista_t*, int);

void check_lista(lista_t*);

void free_lista(lista_t*);

#endif
