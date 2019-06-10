#include "cola.h"

nodo_t* crear_nodo(char* str) {

    //se crea un nuevo nodo y se le asigna memoria
    nodo_t* nuevo_nodo = (nodo_t*)malloc(sizeof(nodo_t));

    //se aloca memoria para el dato del nodo
    nuevo_nodo->dato = (char*)malloc((strlen(str)+1)*sizeof(char));
    strcpy(nuevo_nodo->dato,str);

    nuevo_nodo->sig = NULL;

    return nuevo_nodo;

}

cola_t* crear_cola()  {
    
    //se crea la cola y se asigna memoria para la estructura
    cola_t* cola = (cola_t*)malloc(sizeof(cola_t));
    
    //la cola se inicializa vacia
    cola->pri = NULL;
    cola->ult = NULL;

    return cola;

}

void encolar(cola_t* cola, char* str) {

    nodo_t* nuevo_nodo = crear_nodo(str);
    
    //si la cola esta vacia ult y pri apuntan al mismo
    if(cola->ult == NULL) {

        cola->pri = nuevo_nodo;
        cola->ult = nuevo_nodo;
        return;
    }

    //si la cola ya tenia elementos se agrega el nuevo nodo como siguiente al ult
    cola->ult->sig = nuevo_nodo;
    cola->ult = nuevo_nodo;

}

char* desencolar(cola_t* cola) {

    //si la cola no tiene elementos retorno NULL
    if (cola->pri == NULL)
        return NULL;
    
    //se asigna la memoria necesaria a str para almacenar el dato de la cola
    char* str = (char*)malloc((strlen(cola->pri->dato)+1)*sizeof(char));
    strcpy(str,cola->pri->dato);
    
    //se guarda el puntero al nodo para luego liberarlo
    nodo_t *aux = cola->pri;
    
    //avanza en la cola
    cola->pri = cola->pri->sig;
    
    //libero el nodo desencolado
    free(aux->dato);
    free(aux);

    //si no hay mas elementos al principio la cola esta vacia
    if(cola->pri == NULL)
        cola->ult = NULL;

    return str;
}

void free_cola(cola_t* cola) {
    
    //utilizo el proceso desencolar que desecola y libera el nodo
    char* ptr = desencolar(cola);
    if (ptr == NULL) {
        free(cola);
        return;
    }

    //mientras la cola no sea vacia
    while(ptr != NULL) {

        //libero el puntero a string que aloque al desencolar
        free(ptr);

        ptr = desencolar(cola);
    }

    free(cola);
      
};

