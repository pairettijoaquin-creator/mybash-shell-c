#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <assert.h>

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
    unsigned int length;  // cantidad de argumentos
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
        }
        self->arg = NULL;
        self->input = NULL;
        self->output = NULL;
        self->length = 0;
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
 *
 */
    g_list_free_full(self->arg, g_free); // usa g_free para cada arg de glist
    g_free(self->input);                   // además libero el resto de elementos de scommand
    g_free(self->output);
    g_free(self);
    
   
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
 self->length = self->length+1;
}


/* está mal hecha*/
void scommand_pop_front(scommand self){
    assert(self!=NULL && !scommand_is_empty(self));
/*
 *quita el primer arg (cmd)
 * uso GList* g_list_delete_link (GList* list,  GList* link_)
 * que remueve el nodo "_link" y liberra su memoria
 * 
 * busco que _link sea el primer nodo
 * es decir self->arg->data (data viene de la def de glist)
 * 
*/
scommand link = self->arg->data;
self->arg = g_list_remove_link(self->arg, link);

}

void scommand_set_redir_in(scommand self, char * filename){
    assert(self!=NULL);
    self->input= filename; // establezco filename como input
}

void scommand_set_redir_out(scommand self, char * filename){
    assert(self!=NULL);
    self->output= filename; // establezco filename como output
}

bool scommand_is_empty(const scommand self){
    assert(self!=NULL);
return self->length == 0;
}


unsigned int scommand_length(const scommand self){
    assert(self!=NULL);
return self->length;
}

char * scommand_front(const scommand self){
    assert(self!=NULL && !scommand_is_empty(self));

}

char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);
return self->input; 
}

char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);
return self->output;
}

char * scommand_to_string(const scommand self){
    assert(self!=NULL); 

}

