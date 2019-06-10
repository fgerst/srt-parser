#include "lista.h"

void crear_lista(lista_t* l) {    
    *l = NULL;
}

int es_vacia(lista_t l) {
    return (l == NULL);
}

int buscar(lista_t l, int index) {
    
    while (l) {
        if (l->sub->indice == index) return 1;
        l = l->sig;
    }

    return 0;
}

void insertar_ordenado(lista_t* l, stsubs_t* stsubs) {

    nodol_t *new_nodo = malloc(sizeof(nodol_t));
    new_nodo->sub = stsubs;

    nodol_t *ant = NULL;
    nodol_t *act = *l;

    //busca la posicion donde insertar el subtitulo
    while (act) {
        if (new_nodo->sub->startmilis < act->sub->startmilis)
            break;
        ant = act;
        act = act->sig; 
    }

    //se debe insertar en la primera posicion, o la lista esta vacía, el numero de indice es 1
    if (!ant) {
        new_nodo->sub->indice = 1;
        if (act) {
            while (act) {
                act->sub->indice++;
                act = act->sig;
            }
        }                 
        new_nodo->sig = *l;
        *l = new_nodo;        
    }

    //se debe insertar en una posicion intermedia o en la ultima
    else {
        if (act)
            new_nodo->sub->indice = act->sub->indice;
        else
            new_nodo->sub->indice = ant->sub->indice+1;
        new_nodo->sig = act;
        ant->sig = new_nodo;
        while (act) {
            act->sub->indice++;
            act = act->sig;
        }
    }    
}


void insertar(lista_t* l, stsubs_t* stsubs) {

    nodol_t *new_nodo = malloc(sizeof(nodol_t));
    new_nodo->sub = stsubs;
    new_nodo->sig = NULL;

    if (*l == NULL) *l = new_nodo;

    else {
        nodol_t *aux = *l;
        while (aux->sig) aux = aux->sig;
        aux->sig = new_nodo;
    }
    
}

int eliminar(lista_t* l, int index) {
    
    if (buscar(*l,index)) {

        nodol_t *ant = NULL;
        nodol_t *act = *l;
        nodol_t *aux = NULL;
    
        while (act->sub->indice != index && act) {  
            ant = act;
            act = act->sig;
        }

        if (!ant) *l = act->sig;
        else {
            ant->sig = act->sig;
        }
        
        aux = act->sig;
        while (aux) {
            aux->sub->indice--;
            aux = aux->sig;
        }
        
        free(act->sub->texto);
        free(act->sub);
        free(act);

        return 1;
    }

    return 0;
}

void check_lista(lista_t* l) {
    
    nodol_t* ant;
    nodol_t* act = *l;

    int pri = 1;
    double duracion;
    int diferencia;
    int len;
    int cont_lineas;
    int cont_cars;

    while (act) {
        
        if (pri) {
            if (act->sub->indice != 1)
                puts("El archivo no comienza con indice 1.");
            pri = 0;
        }

        duracion = (act->sub->endmilis - act->sub->startmilis);
        len = strlen(act->sub->texto);

        cont_lineas = 0;
        cont_cars = 0;
        for (int i = 0; i<len; i++) {
            cont_cars++;
            if (act->sub->texto[i] == '\n') { 
                cont_lineas++;;
                if (cont_cars > 38) printf("Indice %d: lineas demasiado largas.\n",act->sub->indice);
                cont_cars = 0;               
            }
            if (cont_lineas > 2) {
                printf("Indice %d: mas de dos lineas de texto.\n", act->sub->indice);          
            }
        }
            
        if ((len*1000) / duracion > 27)
          printf("Indice %d: demasiados caracteres por segundo.\n", act->sub->indice);        

        if (duracion < 1000 || duracion > 7000)
            printf("Indice %d: dura menos de 1 segundo o mas de 7 segundos.\n", act->sub->indice);

        ant = act;
        act = act->sig;
        if (act) {
            if ((act->sub->indice - 1) != ant->sub->indice)
                printf("Indice %d: no consecutivo respecto al anterior.\n", act->sub->indice);
            if (ant->sub->endmilis >= act->sub->startmilis)
                printf("Indice %d y %d: estan solapados.\n", ant->sub->indice, act->sub->indice);
            diferencia = act->sub->startmilis - ant->sub->endmilis;
            if (diferencia < 75)
                printf("Indice %d y %d: menos de 75 ms de diferencia.\n", ant->sub->indice, act->sub->indice);  
        }        
    }
}

void free_lista(lista_t *l) {
    
    nodol_t *nodo = *l;
    while (nodo) {
        nodol_t *aux;
        aux = nodo;
        nodo = nodo->sig;
        free(aux->sub->texto);
        free(aux->sub);
        free(aux);
    }
}

