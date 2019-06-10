/*Procesa los argumentos recibidos en main.
Almacena los nombres de archivos recibidos con '-f' y '-o' en dos arreglos de caracteres.
Las demas operaciones las almacena concatenadas junto con sus opciones en una cola*/

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cola.h"

typedef struct struct_args {
    
    char* filein;
    char* fileout;  

    cola_t* cola_args;

} stargs_t;

//variable global para guardar el codigo de salida.
extern int cod_salida;

//estas tres variables sirven para luego usarlos como condiciones y liberar memoria sin tener 
//errores de uninitialised values.
extern int hay_salida;
extern int hay_entrada;
extern int o_oblig;

//proceso principal, analiza la validez de los argumentos y los guarda.
int procesar_args(int, char**, stargs_t*);

//libera la memoria utilizada para guardar los argumentos.
void free_args(stargs_t*);

#endif

