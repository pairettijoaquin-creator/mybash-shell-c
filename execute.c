#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>  // ésta sirve para los modos de acceso (O_RDONLY, O_WRONLY, O_RDWR, ...)
#include "tests/syscall_mock.h"
#include <glib.h>

#include "command.h"
#include "builtin.h"
#include "execute.h"

 
static void filedesc_redir_input(scommand cmd){
    char* redir_input = scommand_get_redir_in(cmd);
    if (redir_input != NULL){
    int fd = open(redir_input, O_RDONLY, 0);       // se le asigna un int al file descriptor, se abre el stdinput en modo read only
        if (fd == -1) {                       
                perror("error: can't open input");
                exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);             
        close(fd);
    }
}

static void filedesc_redir_output(scommand cmd){
    char* redir_output = scommand_get_redir_out(cmd);
    if (redir_output != NULL){                              
    int fd = open(redir_output,  O_WRONLY|O_CREAT|O_TRUNC, 0640);       // se usan los modos de acceso y un numero que indica los permisos
        if (fd == -1) {                       
                perror("error: can'topen output");
                exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);             
        close(fd);
    }
}

static scommand copy_scommand(scommand sc) {
    scommand aux = scommand_new();
    unsigned int n = scommand_length(sc);

    for (unsigned int i = 0; i < n; i++) {
        char *arg = scommand_front(sc);
        scommand_push_back(aux, strdup(arg)); // duplicamos el string
        scommand_pop_front(sc);
    }

    if (scommand_get_redir_in(sc))
        scommand_set_redir_in(aux, strdup(scommand_get_redir_in(sc)));
    if (scommand_get_redir_out(sc))
        scommand_set_redir_out(aux, strdup(scommand_get_redir_out(sc)));

    return aux;
}


void execute_pipeline(pipeline apipe) {
    assert(apipe != NULL);

    // caso: pipeline vacío (no hay nada que ejecutar)
    if (pipeline_is_empty(apipe)) {
        return;
    }
    
    // caso: pipeline con un solo comando interno, se ejecuta en el proceso padre. 
    if (builtin_alone(apipe)) {              //  pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p))
        scommand cmd = pipeline_front(apipe);
        builtin_run(cmd);   
        return;
    }
    
    // caso: ejecución en segundo plano (background, con '&'), no vamos a esperar a que los hijos terminen.

        unsigned int apipe_length = pipeline_length(apipe);
        
        int pipe_filedes[2];   // guarda file descriptors de cada nuevo pipe (pipe_fildes[0]->lectura, pipe_fildes[1]->escritura)
        int temp[2];           // guarda el pipe anterior (entrada para el próximo comando)
        bool have_temp = false; // indica si temp tiene un pipe válido 
        pid_t *hijos = calloc(apipe_length, sizeof(pid_t));

        for (unsigned int i = 0; i < apipe_length; i++) {
            
            // extrae el comando antes del fork así cada iteración obtiene un comando distinto y el padre avanza la cola. 
            
            scommand cmd = copy_scommand(pipeline_front(apipe));
            pipeline_pop_front(apipe);
            
            // si no es el último comando crea un nuevo pipe
            if (i < apipe_length - 1) {
                pipe(pipe_filedes);
            }

            
            // se crea un nuevo proceso hijo
            int pid = fork();

            if (pid < 0){
                perror("fork");
                exit(EXIT_FAILURE);
            }


            if (pid == 0) {


                // si no es el primer comando conecto stdin con la salida del pipe anterior
            if (have_temp && scommand_get_redir_in(cmd) == NULL) {
                dup2(temp[0], STDIN_FILENO);
                close(temp[0]);
                close(temp[1]);
                }
                
                    

                // si no es el ultimo comando conecto stdout al pipe actual
                if (i < apipe_length - 1  && scommand_get_redir_out(cmd) == NULL) {
                    dup2(pipe_filedes[1], STDOUT_FILENO);
                    close(pipe_filedes[0]);
                    close(pipe_filedes[1]);
                }
                
                filedesc_redir_input(cmd);
                filedesc_redir_output(cmd);


                unsigned int n = scommand_length(cmd);
                char **argv = calloc(n + 1, sizeof(char*)); // +1 para NULL

                for (unsigned int j = 0; j < n; j++) {
                argv[j] = strdup(scommand_front(cmd)); 
                scommand_pop_front(cmd);
                }
                argv[n] = NULL; 

            // ejecutar el comando
            execvp(argv[0], argv);
            perror("execvp");  // solo llega acá si falla
            exit(EXIT_FAILURE);


            } 
            else {
            //Padre
            hijos[i] = pid;
                if (have_temp) {
                    close(temp[0]);
                    close(temp[1]);
                    have_temp = false;
                }
                
                if (i < apipe_length - 1) {
                        temp[0] = pipe_filedes[0];
                        temp[1] = pipe_filedes[1];
                        have_temp = true;
                    }
            scommand_destroy(cmd);
                
        }
        }

    // Caso: ejecución en foreground (sin '&')
if (pipeline_get_wait(apipe)) { 
    for (unsigned int i = 0; i < apipe_length; i++) {
        int status;
        waitpid(hijos[i], &status, 0);
    }
}
    free(hijos);

    }