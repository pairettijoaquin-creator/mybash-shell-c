#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib2.0/glib.h>

#include "command.h"

/* intento crear la estructura para un comando simple,
 * se usaría una doble lista enlazada (Glist) para 
 * y dos punteros de tipo char (redirectores)
 * teniendo en cuenta el ejemplo:
 * ls -l ej1.c > out < in
 * 
 * ls es el primer valor de arg (cmd)
 * -l = arg 1
 * ej1.c = arg2
 * > out escribe el archivo "out"
 * < in lee el archivo "in"
 * 
 * 
 */


struct scommand_s {
    GList* arg;   // comando/argumento
    char* input;  // archivo de entrada (si es necesario)
    char* output; // archivo de salida (si es necesario)
};

 /* glist se define como:
 *
 * typedef struct _GList GList;
 * 
 *   struct _GList {
 *   gpointer data;
 *   GList *next;
 *   GList *prev;
 *  };
 */

scommand scommand_new(void){
    scommand self = NULL;
        self = malloc(sizeof(struct scommand_s));
        
        if (self == NULL) {
    return NULL; 

        self->arg = NULL;
        self->input = NULL;
        self->output = NULL;
    return self;
}


scommand scommand_destroy(scommand self){
    assert(self != NULL);
/*
 * hace falta liberar la memoria de todos los punteros
 * no basta con hacer g_free(self->arg) porque habría memory leaks
 * 
 * uso "g_list_free_full" que libera la memoria de todos
 * los elementos de glist, en vez de solo el primero 
 * 
 * void g_list_free_full (GList* list, GDestroyNotify free_func)
 * toma un puntero de tipo glist (en este caso self->arg)
 * usa g_free para cada arg de glist
 */

    g_list_free_full(self->arg, g_free);
    return self;
}

void scommand_push_back(scommand self, char * argument){
    assert(self!=NULL && argument!=NULL);

/* GList* g_list_append (GList* list, gpointer data)
 * se tiene un puntero de tipo glist (en este caso self->arg)
 * y un puntero a un dato (en este caso sería argument)
 * 
 * entonces busco que se agregue un arg 
 * al final de la lista ya existente
 */

 self->arg = g_list_append (self->arg, argument);
}

void scommand_pop_front(scommand self){
    assert(self!=NULL && !scommand_is_empty(self));
/*
 * se quiere quitar el primer arg de la lista
 * para ello existe:
 * GList* g_list_delete_link (GList* list, GList* link_)
 * donde list es self->arg y link_ el elemento que se quiere eliminar
 * 
 */

}