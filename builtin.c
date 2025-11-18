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




static void exit_run(void){
    printf("\n\n"
       "   █████████                        █████ █████                        \n"
       "  ███▒▒▒▒▒███                      ▒▒███ ▒▒███                         \n"
       " ███     ▒▒▒   ██████   ██████   ███████  ▒███████  █████ ████  ██████ \n"
       "▒███          ███▒▒███ ███▒▒███ ███▒▒███  ▒███▒▒███▒▒███ ▒███  ███▒▒███\n"
       "▒███    █████▒███ ▒███▒███ ▒███▒███ ▒███  ▒███ ▒███ ▒███ ▒███ ▒███████ \n"
       "▒▒███  ▒▒███ ▒███ ▒███▒███ ▒███▒███ ▒███  ▒███ ▒███ ▒███ ▒███ ▒███▒▒▒  \n"
       " ▒▒█████████ ▒▒██████ ▒▒██████ ▒▒████████ ████████  ▒▒███████ ▒▒██████ \n"
       "  ▒▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒   ▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒ ▒▒▒▒▒▒▒▒    ▒▒▒▒▒███  ▒▒▒▒▒▒  \n"
       "                                                     ███ ▒███          \n"
       "                                                    ▒▒██████           \n"
       "                                                     ▒▒▒▒▒▒            \n\n\n");

        exit(EXIT_SUCCESS);
}


// comandos internos disponibles e información de los autores
static void help_run(void){
printf("\n\n"
       " █████   █████          ████           \n"
       "░░███   ░░███          ░░███           \n"
       " ░███    ░███   ██████  ░███  ████████ \n"
       " ░███████████  ███░░███ ░███ ░░███░░███\n"
       " ░███░░░░░███ ░███████  ░███  ░███ ░███\n"
       " ░███    ░███ ░███░░░   ░███  ░███ ░███\n"
       " █████   █████░░██████  █████ ░███████ \n"
       "░░░░░   ░░░░░  ░░░░░░  ░░░░░  ░███░░░  \n"
       "                              ░███     \n"
       "                              █████    \n"
       "                             ░░░░░     \n");


        printf("Integrantes:\n    Bosque Lissandro\n    Galassi Franco\n    Ortega Maximo\n    Pairetti Joaquín\n");
        printf("\nComandos:\n    [cd] = cambia el directorio actual al espeficado\n    [help] = imprime comandos disponibles y nombres de los autores\n    [exit] = termina el proceso\n\n");
}


//cambia el directorio actual
static void cd_run(scommand cmd){
        const char* path = NULL;
        bool too_many = false;

                if (scommand_length(cmd) == 1){                            // si se pone unicamente cd, cambia el directorio a HOME
                    const char* home = getenv("HOME");
                    path = (home != NULL && home[0] != '\0') ? home : "/"; // si no hay HOME, ir a la raíz
                }
                else if (scommand_length(cmd) == 2) {                      // se elimina 1er argumento (cd) y se cambia el directorio a su argumento
                    scommand_pop_front(cmd);
                    path = scommand_front(cmd);      

                } else {                                                   // sobran argumentos, too_many se hace true                 
                    fprintf(stderr, "cd: too many arguments\n");
                    too_many = true;
                }
            
                if (!too_many) {                                           // si too_many es true, da error
                    if (chdir(path) != 0) {
                        perror("cd");
                    }
                }
            }


void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd));
    const char* word = scommand_front(cmd);                             //analiza el comando interno, se ejecuta su función estática
    
    if (strcmp(word, "help") == 0) {
       help_run();

    } else if (strcmp(word, "exit") == 0) {
        exit_run();
    
    } else if (strcmp(word, "cd") == 0) {
        cd_run(cmd);
    }
}

