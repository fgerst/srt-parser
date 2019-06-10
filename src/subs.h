/*Recibe los argumentos procesados, abre el/los archivos y los procesa*/

#ifndef SUBS_H
#define SUBS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "lista.h"
#include "args.h"

typedef struct struct_subs {
    
    int indice;

    long startmilis;
    long endmilis;

    char* texto;

} stsubs_t;
    
typedef struct NodoL *lista_t;

//proceso principal, carga el archivo, lo procesa y lo guarda si es necesario.
int procesar_srt(struct struct_args*);

#endif
