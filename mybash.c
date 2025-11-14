#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"



static void show_prompt(void) {
    char path[1000];
    if(getcwd(path, sizeof(path)) == NULL){
        perror("getcwd() error");
    } 

    printf ("mybash> %s:\033[34m%s$ \033[0m",getenv("USER"), path);
    fflush (stdout);
}

int main(int argc, char *argv[]) {

    printf("\n██████   ██████               ███████████                    █████     \n");
    printf("░██████ ██████               ░░███░░░░░███                  ░░███      \n"); 
    printf("░███░█████░███  █████ ████    ░███    ░███  ██████    █████  ░███████  \n");
    printf("░███░░███ ░███ ░░███ ░███     ░██████████  ░░░░░███  ███░░   ░███░░███  \n");
    printf("░███ ░░░  ░███  ░███ ░███     ░███░░░░░███  ███████ ░░█████  ░███ ░███ \n");
    printf("░███      ░███  ░███ ░███     ░███    ░███ ███░░███  ░░░░███ ░███ ░███ \n");
    printf("█████     █████ ░░███████     ███████████ ░░████████ ██████  ████ █████\n");
    printf("░░░░░     ░░░░░   ░░░░░███    ░░░░░░░░░░░   ░░░░░░░░ ░░░░░░  ░░░░ ░░░░░ \n");
    printf("                  ███ ░███                                              \n");
    printf("                 ░░██████                                               \n");
    printf("                  ░░░░░░                                                \n\n\n\n\n\n");

    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    while (!quit) {
        show_prompt();
        pipe = parse_pipeline(input);

        quit = parser_at_eof(input);
        if (pipe !=NULL) {
                execute_pipeline(pipe);
                pipeline_destroy(pipe);
            }
    }
    parser_destroy(input); input = NULL;
    return EXIT_SUCCESS;
}

