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


void scommand_pop_front(scommand self) {
    assert(self != NULL && !scommand_is_empty(self));
    //chequeo el invariante o sea si self apunta a un comando y self no es vacio, es decir tiene alguin nodo
    
    GList *first = self->arg;          // el nodo a borrar 
    char *arg0 = first->data;          // el dato del nodo (ej, "ls" de tipo char*)

    // liberar el dato que fue alocado dinámicamente
    free(arg0);                        //borro ls

    // libero el nodo, una vez que ya libere el dato aloojado dinamicamente
    self->arg = g_list_delete_link(self->arg, first);

    // mantener invariante de tamaño
    self->length -= 1;
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
/*
* entiendo que se refiere a devolver el primer elemento 
* siguiendo el cuadro de referencia del principio de command.h
* se estaría hablando de "cmd"
* o sea, devuelve self->arg->data
*/
return self->arg->data;
}

char * scommand_get_redir_in(const scommand self){
    assert(self!=NULL);
return self->input; 
}

char * scommand_get_redir_out(const scommand self){
    assert(self!=NULL);
return self->output;
}
char *scommand_to_string(const scommand self) {
    assert(self != NULL);

    if (scommand_is_empty(self)) {
        char *empty = g_malloc0(1);  // string vacío "\0"
        return empty;
    }

    GString *buf = g_string_new(NULL); // Creo un GString vacio para ir armando el resultado.

    /* 1) recorrer la lista de argumentos */
    for (GList *node = self->arg; node != NULL; node = node->next) {//rrecorro nodos
        const char *arg = node->data;

        if (buf->len > 0) { // len es la longitud de la cadena  sin el byte nulo . Si ya hay algo en 'buf', agrego un espacio separador
            g_string_append_c(buf, ' ');//concatena caracter  de espacio al final del buf
        }
        g_string_append(buf, arg);// Concateno el argumento al final del buf
    }

    /* 2) redirecciones si existen */
    const char *in  = scommand_get_redir_in(self);// Nombre de archivo para redirección de entrada o null.
    const char *out = scommand_get_redir_out(self);// Nombre de archivo para redirección de salida o null.

    if (in != NULL) {// si hay redirección de entrada
        g_string_append(buf, " < ");// agrego " < " al texto
        g_string_append(buf, in); // agrego el nombre del archivo de entrada
    }
    if (out != NULL) {// si hay redirección de salida
        g_string_append(buf, " > ");// agrego " > " al texto
        g_string_append(buf, out);// agrego el nombre del archivo de salida
    }

    // devuelvo el buffer como char* (libero memoria con  g_free) 
    return g_string_free(buf, FALSE);
}
////Pipeline////

/* un pipeline es una secuencia de comandos simples conectados por "|" (pipes),
* para la estructura se requiere una lista enlzada de comandos simples
* y un bool que indica si debe esperar que termine para ejecutar el siguiente comando
*
*
*/

// estructura del pipeline //
struct pipeline_s {
    GList* command;    // lista de comandos simples
    bool wait;          // si es true tengo que esperar
    unsigned int length; 
};

pipeline pipeline_new(void) {
    pipeline p = malloc(sizeof(struct pipeline_s));
    assert(p != NULL);
       
    if (p == NULL) {
        return NULL; 
    }

    p->command = NULL;
    p->wait = true;     // por defecto foreground
    p->length = 0;

    assert(pipeline_is_empty(p));
    assert(pipeline_get_wait(p));
    return p;
}

pipeline pipeline_destroy(pipeline self) {
    assert(self != NULL);

    for (GList *n = self->command; n != NULL ; n = n->next) {
        scommand_destroy(n->data);
    }

    g_list_free(self->command);
    free(self);
    return NULL;
}

/// modificadores ///

void pipeline_push_back(pipeline self, scommand sc) {
    assert(self!=NULL && sc!=NULL);
    self->command = g_list_append(self->command, sc);
    (self->length)++;
    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self) {
    assert(self!=NULL && !pipeline_is_empty(self));

    GList* first = self->command;          // el nodo a borrar 
    scommand sc = first->data;         // dato del nodo: un scommand

    scommand_destroy(sc);

    self->command = g_list_delete_link(self->command, first); // nueva cabeza
    self->length -= 1;
}


void pipeline_set_wait(pipeline self, const bool w) {
    assert(self!=NULL);
    self->wait = w;
}


/// proyectores ///

bool pipeline_is_empty(const pipeline self) {
    assert(self!=NULL);
    return self->length == 0;
}

unsigned int pipeline_length(const pipeline self) {
    assert(self!=NULL);
    return self->length;
}

scommand pipeline_front(const pipeline self) {
    assert(self != NULL && !pipeline_is_empty(self));
    return (scommand) self->command->data; 
}

bool pipeline_get_wait(const pipeline self) {
    assert(self!=NULL);
    return self->wait;
}

char * pipeline_to_string(const pipeline self) {
     assert(self != NULL);

    if (pipeline_is_empty(self)) {// caso borde: pipeline vacío  devolver string vacío
        char *empty = g_malloc0(1);  // devuelve "\0"
        return empty;
    }

    GString *buf = g_string_new(NULL);

    // 1) recorrer la lista de scommand
    for (GList *node = self->command; node != NULL; node = node->next) {
        scommand sc = (scommand) node->data;// convertir cada scommand a string usando la función que ya hiciste
        char *sc_str = scommand_to_string(sc);

        if (buf->len > 0) {
            g_string_append(buf, " | ");  // separar con pipe
        }
        g_string_append(buf, sc_str);
        g_free(sc_str); // liberás el char* devuelto por scommand_to_string
    }

    //  si el pipeline está en background (wait == false), agregás " &"
    if (!pipeline_get_wait(self)) {
        g_string_append(buf, " &");
    }

    //  devolver char* plano (liberable con g_free por el caller)
    return g_string_free(buf, FALSE);
}


