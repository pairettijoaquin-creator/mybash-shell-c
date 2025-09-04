#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    scommand sc = scommand_new();
    bool error = false;             // corresponde a si da error en el parseo (algo invalido)
    bool finish = false;            // corresponde a si se encontro un (|) o (\n) 
    bool have_word = false;         // corresponde a si en el comando hay al menos una palabra
    
    while(!error && !finish){
        arg_kind_t kind;
        char* arg = parser_next_argument(p, &kind);
        
        if (arg == NULL) {                              // caso 1) chequeo si estoy en (|) o en (\n)
            finish = true;
        } 
        else if (kind == ARG_NORMAL) {                  // caso 2) encontre una palabra normal -> agrego a mi sc ese arg
            scommand_push_back(sc, arg);
            have_word = true;
        }
        else if (kind == ARG_INPUT) {                   // caso 3) encontre un redireccion de entrada (<) -> agrego en mi sc un input
            if (scommand_get_redir_in(sc) != NULL) { 
                error = true;
                free(arg);
            } else {scommand_set_redir_in(sc, arg);}
        }
        else if (kind == ARG_OUTPUT) {                  // caso 4) encontre una redireccion de salida (>) que debo hacer? -> agrego en mi sc un output 
            if (scommand_get_redir_out(sc) != NULL){
                error = true;
                free(arg);
            } else {scommand_set_redir_out(sc, arg);}
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
    bool error = false;
    bool another_pipe = true;

    cmd = parse_scommand(p);
    error = (cmd==NULL);                                // Comando inválido al empezar
    while (another_pipe && !error) {
        pipeline_push_back(result, cmd);
        parser_op_pipe(p, &another_pipe);               // tengo que ver si tengo otro pipe
        //scommand_destroy(cmd); IMPORTANTE CONSULTAR ESTO 
        if (another_pipe) {
            cmd = parse_scommand(p);
            error = (cmd==NULL);
        }
    }

    return result;
}

