#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    scommand sc = NULL;
    bool error = false;
    bool finish = false;
    bool have_word = false;

    while (!error && !finish) {
        arg_kind_t kind;
        char *arg = parser_next_argument(p, &kind);

        if (arg == NULL) {                          // Encontró '|' o '\n' y NO los consumió: acá termina este scommand
            finish = true;
        } else if (kind == ARG_NORMAL) {
            if (sc == NULL) {sc = scommand_new();}
            scommand_push_back(sc, arg);
            have_word = true;
        } else if (kind == ARG_INPUT) {
            if (sc == NULL) { 
                error = true; free(arg); 
            }
            else if (scommand_get_redir_in(sc) != NULL) {
                error = true; free(arg); 
                }
            else { 
                scommand_set_redir_in(sc, arg); 
            }
        } else if (kind == ARG_OUTPUT) {
            if (sc == NULL) {
                error = true;
                free(arg); 
            }
            else if (scommand_get_redir_out(sc) != NULL) {
                error = true;
                free(arg); 
            }
            else {
                scommand_set_redir_out(sc, arg); 
            }
        }
    }

    if (error || !have_word) {
        if (sc) sc = scommand_destroy(sc);
        return NULL;
    }
    return sc;
}

pipeline parse_pipeline(Parser p) {
    assert(p != NULL);
    assert(! parser_at_eof (p));
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe=true;
    
    cmd = parse_scommand(p);

    error = (cmd==NULL); 
    while (another_pipe && !error) {
        pipeline_push_back(result, cmd);        // 1) guardar el comando actual en el pipeline 
        parser_op_pipe(p, &another_pipe);       // 2) ver si hay otro '|' y consumirlo si existe 

        if (another_pipe) {                     // 2) ver si hay otro '|' y consumirlo si existe
            cmd = parse_scommand(p);
            error = (cmd == NULL);              // pipe sin comando a la derecha => error 
        }   
    }
    
    parser_skip_blanks(p);
    bool background = false; 
    parser_op_background(p, &background);
    pipeline_set_wait(result, !background);     // si hay & no espero, y viceversa

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