#include "src/args.h"
#include "src/subs.h"

int hay_salida = 0;
int hay_entrada = 0;
int o_oblig = 0;

int cod_salida = 0;

void print_ayuda() {

    printf("Codigo de salida: %d\n", cod_salida);
    puts("\nParametros validos:\n-f 'filein' (obligatorio).");
    puts("-o 'fileout' (obligatorio si se modifica el archivo).");
    puts("-i [startmilis] [endmilis] 'texto'.\n-b [indice].\n-v");

}

int main(int argc, char *argv[]) {
          
    //verifica que se haya ingresado algun parametro.
    if (argc < 2) {
        puts("\nERROR: Ingrese parametros.");
        print_ayuda(2);
        return 2;
    }
     
    stargs_t *stargs = malloc(sizeof(stargs_t));

    //si los parametros son validos comienza a manipular el archivo srt.
    if (procesar_args(argc, argv, stargs)) { 

        puts("\n*********************");
        puts("Parametros procesados.\n"); 

        if (procesar_srt(stargs)) {
            puts("\n*********************");
            puts("Archivos procesados.\n");
        }
        else 
            printf("Codigo de salida: %d\n", cod_salida);
    } else
        print_ayuda();

    free_args(stargs);
    
    return cod_salida;

}
