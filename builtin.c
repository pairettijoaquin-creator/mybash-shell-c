#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <assert.h>
#include <string.h>

#include "command.h"

/* comienzo con los comandos internos (cd, help, exit)*/


/* void builtin_cd(scommand comando){
   if (comando->arg->data == NULL){  // se ve si el comando es vacío
        chdir(no sé);
    } else {
       chdir()}    // si no es vacío, se ve el input

    }
*/




void builtin_help(void){
char * helptxt = NULL;
    helptxt = 
    
    /*
    * nombres: .....
    *
    *
    * comandos: 
    *   cd = cambia el directorio actual al espeficado
        help = imprime comandos disponibles y nombres de los autores
        exit = termina el proceso
    */

    printf("%s", helptxt);
}

void builtin_exit(void){
    exit(EXIT_SUCCESS);
}


bool builtin_is_internal(scommand cmd){
assert(cmd != NULL);

return strcmp(scommand_front(cmd), "cd") == 0 || strcmp(scommand_front(cmd), "help") == 0 || strcmp(scommand_front(cmd), "exit") == 0;
}

bool builtin_alone(pipeline p){
assert(p != NULL);

assert(builtin_alone(p) == pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p)));
}


void builtin_run(scommand cmd){
    assert{builtin_is_internal(cmd);
    
    /* acá sería ver cual comando es y usar las void builtin_(comando interno)
    * sería algo como 

        if (strcmp(scommand_front(cmd), "help") {
        builtin_help();
       } else if (strcmp(scommand_front(cmd), "exit") {
        builtin_exit();
        } else if (strcmp(scommand_front(cmd), "cd") {
         builtin_cd(cmd);
    
    *
    */
    

}
