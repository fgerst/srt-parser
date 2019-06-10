#include "subs.h"

//chequea errores al obtener un numero con strtol
static int check_val(long, char*, char*);

//procesa la linea del indice
static int parse_indice(char*, stsubs_t*);

//procesa la linea donde esta [startmilis] --> [endmilis]
static int parse_tiempo(char*, stsubs_t*);

//recibe el nombre del archivo a procesar y lo carga en una lista
static int cargar_srt(char*, lista_t*);

//aplica las acciones representadas por los parametros ingresados
static int procesar_lista(cola_t*, lista_t*);

//recibe la lista procesada y guarda el archivo con el nombre recibido
static int guardar_srt(char*, lista_t);


static int check_val(long val, char* line, char* endptr) {
    
    errno = 0;
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0)) {
        return 0;
    }
    if (line == endptr) {
        return 0;
    }
  
    return 1;
}

static int parse_indice(char* buf, stsubs_t* stsubs) {
    
    static int primer_indice = 1;   

    char* endptr;
    long val;

    val = strtol(buf,&endptr,10);

    if (primer_indice == 1 && !check_val(val,buf,endptr)) {
        //Este intento de lectura adicional es porque en algunas codificaciones, al principio del archivo
        //hay algunos caracteres especiales(se ven usando cat -v), lo que hace que strtol no
        //pueda obtener el primer indice. En los .srt que pasaba esto, avanzar 3 posiciones con el puntero
        //hace que quede el numero. De todas formas puede haber errores y por eso se puede elegir si hacerlo o no.
        puts("*Error al obtener el primer indice*");
        puts("Para intentar ignorar caracteres de codificacion ingrese 's'.");
        puts("Para salir ingrese cualquier otra tecla");
        primer_indice = 0;
        if (getchar() == 's') {
            buf += 3; ; 
            val = strtol(buf,&endptr,10);
        }
        if (check_val(val,buf,endptr))
            puts("\nPrimer indice leído.. Continuando.\n");
            
    }

    if (!check_val(val,buf,endptr)) {
        puts("Error al obtener un indice");
        return 0;
    }

    
    primer_indice = 0;
    stsubs->indice = (int) val;

    return 1;
}

static int parse_tiempo(char* buffer, stsubs_t* stsubs) {

    char* aux = buffer;
    char* endptr;
    long val;

    int start[4];
    int end[4];
    
    //se guarda el tiempo de inicio, avanza 5 posiciones (salta el " --> ") y se guarda el tiempo de final    
    for (int i = 0; i < 4; i++) {

        val = strtol(aux,&endptr,10);

        if (!check_val(val,aux,endptr)) {
            return 0;
        }

        start[i] = val;
        aux = endptr + 1;
    }

    aux = endptr + 5;

    for (int i = 0; i < 4; i++) {

        val = strtol(aux,&endptr,10);

        if (!check_val(val,aux,endptr)) {
            return 0;
        }        

        end[i] = val;
        aux = endptr + 1;
    }

    stsubs->startmilis = (start[0]*60*60*1000)+(start[1]*60*1000)+(start[2]*1000)+start[3];
    stsubs->endmilis = (end[0]*60*60*1000)+(end[1]*60*1000)+(end[2]*1000)+end[3];

    return 1;

} 

static int cargar_srt(char* filein, lista_t* lista) {
    
    FILE* f = fopen(filein, "r");
    if (f == NULL) {
        puts("*No se pudo acceder al archivo de entrada");
        cod_salida = 4;
        return 0;
        
    }

    crear_lista(lista);

    char* buffer = NULL;
    size_t len = 0;
    ssize_t read;

    //lee lineas hasta llegar al final del archivo
    while ((read = getline(&buffer, &len, f)) != -1) {
        
        int i = 0;
        int realoc_texto = 0;
        stsubs_t *stsubs = malloc(sizeof(stsubs_t));
        
        //el archivo empieza con una linea vacia o hay dos lineas vacias consecutivas en algun momento
        if (buffer[0] == '\n' || (buffer[0] == '\x0d' && buffer[1] == '\n')) {
            puts("El subtitulo comienza con una linea vacia o hay dos lineas vacias consecutivas.");
            free(stsubs);
            fclose(f);
            free(buffer);
            return 0;
        }

        //switch con el fin de en las primeras dos pasadas guardar indice y tiempo, y en las restantes el texto
        do {
            switch(i)
            {
                case 0:
                    {
                    if (!parse_indice(buffer,stsubs)){
                        free(buffer);
                        free(stsubs);
                        fclose(f);
                        puts("\n*Error al obtener indice*\nError de sintaxis o no es un numero.\n");
                        return 0;
                    }
                    break;
                    }
                case 1:
                    {
                    if (!parse_tiempo(buffer,stsubs)) {
                        free(stsubs);
                        free(buffer);
                        fclose(f);
                        puts("\n*Error al obtener una linea de [startmilis] [endmilis]*\nError de sintaxis o no es un numero.\n");
                        return 0;
                    }
                    break;
                    }
                default:
                    {
                    if (realoc_texto == 0) {
                        stsubs->texto = malloc((strlen(buffer)+1)*sizeof(char));
                        strcpy(stsubs->texto, buffer);
                    } 
                    else {
                        stsubs->texto = realloc(stsubs->texto,
                            (strlen(stsubs->texto)+strlen(buffer)+1)*sizeof(char));
                        strcat(stsubs->texto,buffer);
                    }   

                    realoc_texto = 1;
                                        
                    }                   

            }
    
            i++;
        
        //corte de control del subtitulo actual se da cuando la linea es vacía (en algunas codificaciones hay un ^M antes del \n)
        } while ((read = getline(&buffer, &len, f)) != -1 && buffer[0] != '\n' && !(buffer[0] == '\x0d' && buffer[1] == '\n'));
        
        //a un subtitulo le falta una linea (minimo 3: indice, tiempo, texto)
        if (i < 3) {
            puts("El subtitulo tiene errores de sintaxis");
            free(stsubs);
            fclose(f);
            free(buffer);
            return 0;
        }

        //si el subtitulo actual es correcto, se almacena en la lista
        insertar(lista,stsubs);
 
        //corte de control por si se llego al fin de archivo
        if (read == -1) break;

    }
    
    free(buffer);
    fclose(f);
    
    return 1;

}

static int procesar_lista(cola_t* cola, lista_t* lista) {

    char* arg = desencolar(cola);

    while (arg != NULL) {

        //switch para procesar cada argumento, si un argumento no es valido, lo saltea y sigue con el siguiente
        switch(arg[1])
        {
            //case -i parsea [stmilis] [endmilis] "texto" del argumento, lo almacena en la estructura y lo inserta
            case 'i':
            {   
                char* endptr, *aux;
                long stmilis, endmilis;

                aux = arg;
                aux += 2;
                stmilis = strtol(aux,&endptr,10);

                if (!check_val(stmilis,aux,endptr)) {
                    puts("\nError en el argumento 'i'. [stmilis] no es valido.\n");
                    break; 
                }
        
                aux = endptr;
                endmilis = strtol(aux,&endptr,10);
                if (!check_val(stmilis,aux,endptr)) {
                    puts("\nError en el argumento 'i'. [endmilis] no es valido.\n"); 
                    break;
                }

                if ((stmilis < 0 || endmilis < 0) || endmilis < stmilis) {
                    puts("Error en el argumento 'i'. [startmilis] o [endmilis] no son validos.");
                    break;                   
                }
    
                aux = ++endptr;
      
                stsubs_t *stsubs = malloc(sizeof(stsubs_t));
                stsubs->startmilis = stmilis;
                stsubs->endmilis = endmilis;
                stsubs->texto = malloc((strlen(aux)+2)*sizeof(char));
                strcpy(stsubs->texto,aux);
                strcat(stsubs->texto,"\n");
                
                insertar_ordenado(lista,stsubs);                
    
                break;
            }
            //case -b parsea el indice e intenta eliminarlo de la lista
            case 'b':
            {
                char* endptr, *aux;
                int indice;
                
                aux = arg;
                aux += 2;
                indice = strtol(aux,&endptr,10);

                if (!check_val(indice,aux,endptr) || indice < 1) {
                    puts("\nError en el argumento '-b'. [indice] no es valido.");
                    break;
                }
                if (!eliminar(lista, indice)) {
                    printf("El indice numero %d no existe en el archivo\n", indice);
                    break;
                }
                break;
            }
            //case -v verifica los subtitulos
            case 'v':
            {
                check_lista(lista);
                puts("\nArchivo verificado");
                break;
            }
            default:
            {
                puts("Hubo un error al procesar un parametro.\n");
                break;
            } 
        }

        free(arg);
        arg = desencolar(cola);  
    }    

    return 1;

}

static int guardar_srt(char* fileout, lista_t lista) {
    
    //Si ya existe un archivo con el mismo nombre de salida, lo elimina. 
    //Esto es para evitar que el archivo de entrada abierto con una codificacion X, se guarde en una codificacion Y
    //debido a que antes se haya ejectutado el programa con con un archivo de entrada de codificacion Y.
    //Ya que el flag "w" de fopen no crea un archivo nuevo sino que trunca el ya existente que puede estar en una
    //codificacion diferente. 
    remove(fileout);
    
    FILE *f = fopen(fileout, "w");
    if (f == NULL) {
        puts("No se pudo acceder al archivo de salida");
        return 0;
    }

    int ms, s, m, h;
    int y = 60*60*1000;
    long x;

    //vuelca la lista con formato en el archivo de salida 
    while (lista) {     
 
        fprintf(f, "%d\n", lista->sub->indice);

        x = lista->sub->startmilis;         
        h = x/y;
        m = (x-(h*y))/(y/60);
        s = (x-(h*y)-(m*(y/60)))/1000;
        ms = x-(h*y)-(m*(y/60))-(s*1000);
        fprintf(f, "%02d:%02d:%02d,%03d --> ", h, m, s, ms);
   
        x = lista->sub->endmilis;
        h = x/y;
        m = (x-(h*y))/(y/60);
        s = (x-(h*y)-(m*(y/60)))/1000;
        ms = x-(h*y)-(m*(y/60))-(s*1000);
        fprintf(f, "%02d:%02d:%02d,%03d\n", h, m, s, ms);

        fprintf(f, "%s\n", lista->sub->texto);  
        lista = lista->sig;

    }

    fclose(f);
    
    return 1;

}

int procesar_srt(stargs_t* stargs) {

    lista_t lista; 
 
    //si se pudo cargar el archivo en la lista, lo procesa.        
    if (cargar_srt(stargs->filein, &lista))
        printf("*Archivo de entrada: '%s'.\n", stargs->filein);
    else {
        printf("Hubo un error al cargar archivo: '%s'\n", stargs->filein);
        if (cod_salida != 4) free_lista(&lista);
        return 0;
    }

    procesar_lista(stargs->cola_args,&lista);

    //si el archivo fue modificado, o si se especifico un archivo de salida, lo vuelca en el archivo de salida
    if (hay_salida && o_oblig) {
        if (guardar_srt(stargs->fileout, lista))
            printf("*Cambios guardados en: '%s'.\n", stargs->fileout);
        else {
            printf("Hubo un error al guardar el archivo: %s\n", stargs->fileout);
            free_lista(&lista);
            return 0;    
        }
    }

    free_lista(&lista);
    
    //en este punto el proceso fue exitoso
    
    return 1;

}
