#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>  // ésta sirve para los modos de acceso (O_RDONLY, O_WRONLY, O_RDWR, ...)


#include "command.h"
#include "builtin.h"


 
static void filedesc_redir_input(scommand cmd){
    char* redir_input = scommand_get_redir_in(cmd);
    if (redir_input != NULL); 
    int fd = open(redir_input, O_RDONLY);       // se le asigna un int al file descriptor, se abre el stdinput en modo read only
        if (fd == -1) {                       
                perror("error: can't open input");
                exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);             
        close(fd);
}

static void filedesc_redir_output(scommand cmd){
    char* redir_output = scommand_get_redir_in(cmd);
    if (redir_output != NULL);                               
    int fd = open(redir_output,  O_WRONLY|O_CREAT|O_TRUNC, 0640);       // se usan los modos de acceso y un numero que indica los permisos
        if (fd == -1) {                       
                perror("error: can'topen output");
                exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);             
        close(fd);
}

void execute_pipeline(pipeline apipe) {
    assert(apipe != NULL);

    // caso: pipeline vacío (no hay nada que ejecutar)
    if (pipeline_is_empty(apipe)) {
        return;
    }
    
    // caso: pipeline con un solo comando interno, se ejecuta en el proceso padre. 
    if (builtin_alone(apipe)) {              //  pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p))
        builtin_run(pipeline_front(apipe));   
        return;
    }
    
    // caso: ejecución en segundo plano (background, con '&'), no vamos a esperar a que los hijos terminen.
    if (!pipeline_get_wait(apipe)) {
        
        unsigned int apipe_length = pipeline_length(apipe);
        
        int pipe_filedes[2];   // guarda file descriptors de cada nuevo pipe (pipe_fildes[0]->lectura, pipe_fildes[1]->escritura)
        int temp[2];           // guarda el pipe anterior (entrada para el próximo comando)

        for (unsigned int i = 0; i < apipe_length; i++) {
            
            // si no es el primer comando hereda el pipe anterior
            if (i != 0) {
                temp[0] = pipe_filedes[0];
                temp[1] = pipe_filedes[1];
            }
            
            // si no es el último comando crea un nuevo pipe
            if (i < apipe_length - 1) {
                pipe(pipe_filedes);
            }
            
            // se crea un nuevo proceso hijo
            int pid = fork();

            if (pid == 0) {
                scommand cmd = pipeline_front(apipe);
                pipeline_pop_front(apipe);

                // si no es el primer comando conecto stdin con la salida del pipe anterior
                if (i > 0) {
                    dup2(temp[0], STDIN_FILENO);
                    close(temp[0]);
                    close(temp[1]);
                }
                
                // si no es el ultimo comando conecto stdout al pipe actual
                if (i < apipe_length - 1) {
                    dup2(pipe_filedes[1], STDOUT_FILENO);
                    close(pipe_filedes[1]);
                    close(pipe_filedes[0]);
                }
                
                // aplico las redirecciones
                filedesc_redir_input(cmd);
                filedesc_redir_output(cmd);

                /*
                *  aca iria la ejecucion del cmd
                *
                * 
                * 
                */
            
            } 
            else if (pid > 0) {
                
                if (i > 0) {
                    close(temp[0]);
                    close(temp[1]);
                }
                
            } 
            else {
                // fork() falló
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Caso: ejecución en foreground (sin '&')
    if (pipeline_get_wait(apipe)) {
        /*
        *   esperaa a que todos los hijos terminen
        */
    }
}

