#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "tests/syscall_mock.h"

#include "command.h"
#include "builtin.h"

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);
    
    if (scommand_is_empty(cmd)) return false;                           // sin palabras, no es builtin
    const char* word = scommand_front(cmd);
    
    return (strcmp(word, "cd") == 0)  ||
           (strcmp(word, "help") == 0)||
           (strcmp(word, "exit") == 0);
}

bool builtin_alone(pipeline p){ 
    assert(p != NULL); 
    
    return pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p)); 
}

void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd));
    const char* word = scommand_front(cmd);                             //tomo el comando interno
    
    if (strcmp(word, "help") == 0) {
        printf("Shell MyBash\n");
        printf("integrantes:\n Bosque Lissandro\n Galassi Franco \n Ortega Maximo\n Pairetti Joaquín\n");
        printf("Comandos:\n cd = cambia el directorio actual al espeficado\nhelp = imprime comandos disponibles y nombres de los autores\nexit = termina el proceso\n");
    
    } else if (strcmp(word, "exit") == 0) {
        exit(EXIT_SUCCESS);
    
    } else if (strcmp(word, "cd") == 0) {
        const char* path = NULL;
        bool too_many = false;

        if (scommand_length(cmd) == 2) {
            scommand_pop_front(cmd);
            path = scommand_front(cmd);                                 // 1er argumento

        } else {
            const char* home = getenv("HOME");
            path = (home != NULL && home[0] != '\0') ? home : "/";      // si no hay HOME, ir a la raíz
    
            if (!scommand_is_empty(cmd)) {                              // sobran argumentos
                fprintf(stderr, "cd: too many arguments\n");
                too_many = true;
            }
        }
        if (!too_many) {
            if (chdir(path) != 0) {
                perror("cd");
            }
        }
    }
}

