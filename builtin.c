#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <assert.h>
#include <string.h>

#include "command.h"


bool builtin_is_internal(scommand cmd){
assert(cmd != NULL);

return strcmp(scommand_front(cmd), "cd") == 0 || strcmp(scommand_front(cmd), "help") == 0 || strcmp(scommand_front(cmd), "exit") == 0;
}

bool builtin_alone(pipeline p){
assert(p != NULL);

return(builtin_alone(p) == pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p)));
}


void builtin_run(scommand cmd){
    assertbuiltin_is_internal(cmd);

        if (strcmp(scommand_front(cmd), "help")) {
            printf("Shell "MyBash"\n");
            printf("integrantes:\n\n Bosque Lissandro\n Galassi Franco \n Ortega Maximo\n Pairetti Joaquín");
            printf("Comandos:\n\n"cd" = cambia el directorio actual al espeficado\n"help" = imprime comandos disponibles y nombres de los autores\n"exit" = termina el proceso");

       } else if (strcmp(scommand_front(cmd), "exit")) {
        exit(EXIT_SUCCESS);
        } else if (strcmp(scommand_front(cmd), "cd")) {
            char* comando = NULL;
            if (scommand_length(cmd)==2){
                comando = g_list_nth_data(cmd->arg, 1)
        }   else {
                comando = chdir(getenv("HOME"));
        }
    }

}
