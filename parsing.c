#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    scommand sc = scommand_new();
    bool error = false;             // corresponde a si da error en el parseo (algo invalido)
    bool finish = false;            // corresponde a si se encontro un (|) o (\n) 
    bool have_word = false;         // corresponde a si en el comando hay al menos una palabra, no importa el lugar, es una sintaxis valida para la shell
    
    while(!error && !finish){
        arg_kind_t kind;
        char* arg = parser_next_argument(p, &kind);
        
        // en arg tengo una palabra o redireccion, kind me indica de que tipo es
        // ahora debo ver por casos que es lo que tomo parser_next_argument(p, &kind)
        
        if (arg == NULL) {                              // caso 1) chequeo si estoy en (|) o en (\n)
            finish = true;
        } 
        else if (kind == ARG_NORMAL) {                  //caso 2) encontre una palabra normal -> agrego a mi sc ese arg
            scommand_push_back(sc, arg);
            have_word = true;
        }
        else if (kind == ARG_INPUT) {                   //caso 3) encontre un redireccion de entrada (<) -> agrego en mi sc un input
            if (scommand_get_redir_in(sc) != NULL) {    // si ya habia redireccion de entrada, error
                error = true;
                free(arg);
            } else {scommand_set_redir_in(sc, arg);};   // si no, lo seteo
        }
        else if (kind = ARG_OUTPUT) {                   //caso 4) encontre una redireccion de salida (>) que debo hacer? -> agrego en mi sc un output 
            if (scommand_get_redir_out(sc) != NULL) {   // si ya habia redireccion de salida, error
                error = true;
                free(arg);
            } else {scommand_set_redir_out(sc, arg);};  // si no, lo seteo
        }
    }

    if (error == true || have_word == false) {          // una vez terminado el ciclo chequeo si salto algun error
        sc = scommand_destroy(sc);
    }

    return sc;
}

pipeline parse_pipeline(Parser p) {
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe=true;

    cmd = parse_scommand(p);
    error = (cmd==NULL); 
   while (another_pipe && !error) {
    pipeline_push_back(result, cmd);// 1) guardar el comando actual en el pipeline 
    parser_op_pipe(p, &another_pipe);  // 2) ver si hay otro '|' y consumirlo si existe 

    if (another_pipe) { // 2) ver si hay otro '|' y consumirlo si existe
        cmd = parse_scommand(p);
        error = (cmd == NULL);  // pipe sin comando a la derecha => error 
        } 
   }
    parser_skip_blanks(p);
    bool background = false; 
    parser_op_background(p, &background);
    pipeline_set_wait(result, !background);             // si hay & no espero, y viceversa

    if (!parser_at_eof(p)) {
        bool gar = false;
        parser_garbage(p, &gar);
        if(gar) {
            error = true;
        }
    }

    if (error || pipeline_length(result) == 0) {
        result = pipeline_destroy(result);
    }
return result;

}