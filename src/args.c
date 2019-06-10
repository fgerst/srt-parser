#include "args.h"

//recibe el nombre del archivo de entrada y lo guarda.
static int parse_filein(stargs_t* stargs, char* ptr);

//recibe el nombre del archivo de salida y lo guarda.
static int parse_fileout(stargs_t*, char*);

//parse_opts recibe los argumentos '-i' '-b' y '-v' y los encola.
static int parse_opts(cola_t*, char**, int, int);


static int parse_filein(stargs_t* stargs, char* ptr) {
    
    //verifica que el parametro -f este seguido de una opcion.
    if (ptr) {

        //aloca la memoria necesaria en el estructura y copia el nombre del archivo de entrada.
        stargs->filein = (char*)malloc((strlen(ptr)+1)*sizeof(char));
        strcpy(stargs->filein,ptr);

        return 1;

    }
    
    puts("\nERROR: Sintaxis de '-f' incorrecta.");
    return 0;

}

static int parse_fileout(stargs_t* stargs, char* ptr) {
    
    if (ptr) {
        stargs->fileout = (char*)malloc((strlen(ptr)+1)*sizeof(char));
        strcpy(stargs->fileout,ptr);

        return 1;
    }

    puts("\nERROR: Sintaxis de '-o' incorrecta.");
    return 0;

}


static int parse_opts(cola_t* cola, char* ptr[], int pos, int ident) {
    
    //aloca memoria suficiente para '-x' y un espacio.
    char* tmp = (char*)malloc(4 * sizeof(char));  
    
    //espacio es utilizado como separador entre parametro y opciones.
    char* espacio = " ";

    strcpy(tmp,ptr[pos]);
    strcat(tmp,espacio);
    
    pos++;
    //itera si es que el argumento requiere de opciones.
    for (int i = 1; i <= ident; i++) {
        
        //verifica que se haya ingresado la siguiente opcion
        if (ptr[pos] == NULL) {
            puts("\nERROR: Faltan opciones en algun parametro.");
            free(tmp);
            return 0;
        }
  
        //aloca memoria suficiente para la proxima opcion.  
        tmp = (char*)realloc(tmp, (strlen(tmp) + strlen(ptr[pos]) + 3) *sizeof(char));

        //concatena en tmp el parametro y su opcion siguiente.
        strcat(tmp, ptr[pos]);
        strcat(tmp, espacio);

        pos++;
                               
    }

    //encola parametro y sus opciones.
    encolar(cola,tmp);

    free(tmp);

    return 1;

}


int procesar_args(int argc, char *argv[], stargs_t* stargs) {
    
    unsigned int f_cont = 0;
    unsigned int o_cont = 0;

    stargs->cola_args = crear_cola();
    
    for (int i = 1; i < argc; i++) {
        
        //verifica que los parametro comiencen con '-'.
        if (argv[i][0] != '-') {   
            puts("\nERROR: No ingresó cada parametro precedido por un '-'.");
            cod_salida = 3;
            return 0;
        }

        //si parametro actual comienza con '-' pero su longitud no es 2 no es valido.
        if (strlen(argv[i]) != 2) {
            printf("\nERROR: El parametro %s es inválido.\n", argv[i]);
            cod_salida = 3;
            return 0;
        }
        
        switch(argv[i][1])
        {       
            case 'f':
                f_cont++;
                if (f_cont > 1) {    
                    puts("\nERROR: Solo un parametro -f permitido.");
                    cod_salida = 1;           
                    return 0;
                }
                if (!parse_filein(stargs,argv[++i])) {
                    cod_salida = 3;
                    return 0;
                }
                hay_entrada = 1;
                break;

            case 'o':
                o_cont++;
                if (o_cont > 1) {
                    puts("\nERROR: Solo un parametro -o permitido.");
                    cod_salida = 1;
                    return 0;
                }
                if (!parse_fileout(stargs,argv[++i])) {
                    cod_salida = 3;
                    return 0;
                }
                hay_salida = 1;
                break;

            case 'b':
                o_oblig = 1;
                if (!parse_opts(stargs->cola_args,argv,i,1)) {
                    cod_salida = 3;
                    return 0;
                }
                i++;
                break;

            case 'i':
                o_oblig = 1;
               
                if (!parse_opts(stargs->cola_args,argv,i,3)) {
                    cod_salida = 3;
                    return 0;
                }
                i += 3;
                break;

            case 'v':
                if (!parse_opts(stargs->cola_args,argv,i,0)) {
                    cod_salida = 3;
                    return 0;
                }
                break;

            default:
                printf("\nERROR: El parametro %s es invalido.\n", argv[i]);
                cod_salida = 3;
                return 0;   
        }

    }

    
    //verifica que '-f' haya sido ingresado.
    if (f_cont == 0) {   
        puts("\nERROR: Parametro '-f' es requerido.");
        cod_salida = 2;
        return 0;
    }
    
    //verifica que '-o' haya sido ingresado en caso de ser obligatorio.
    if (o_oblig && o_cont == 0) {
        puts("\nERROR: Parametro '-o' requerido.");
        cod_salida = 2;
        return 0;
    }

    return 1;

}

void free_args(stargs_t* stargs) {
    
    if (hay_entrada)
        free(stargs->filein);
    if (hay_salida)
        free(stargs->fileout);
    free_cola(stargs->cola_args);
    free(stargs);
    
}
