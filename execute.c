#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>

#include "command.h"
#include "builtin.h"


void execute_pipeline(pipeline apipe) {
    assert(apipe != NULL);

    // caso: pipeline vacío (no hay nada que ejecutar)
    if (pipeline_is_empty(apipe)) {
        return;
    }
    
    // caso: pipeline con un solo comando interno, se ejecuta en el proceso padre. 
    if (builtin_alone(apipe)) {
        builtin_run(pipeline_front(apipe));
        return;
    }
    
    // caso: ejecución en segundo plano (background, con '&'), no vamos a esperar a que los hijos terminen.
    if (!pipeline_get_wait(apipe)) {
        
        unsigned int apipe_length = pipeline_length(apipe);
        
        int pipe_filedes[2];   // guarda file descriptors de cada nuevo pipe
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
                /*
                *   proceso hijo
                * 
                */
            } 
            else if (pid > 0) {
                /*
                *   proceso padre
                * 
                */
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

