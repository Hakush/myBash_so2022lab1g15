#include <stdlib.h>
#include <stdbool.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    scommand result = scommand_new();
    arg_kind_t type;
    char * current_arg = parser_next_argument(p, &type);
    while (current_arg != NULL){
        parser_skip_blanks(p);
        if (type == ARG_NORMAL){
            scommand_push_back(result, current_arg);
        }
        if (type == ARG_OUTPUT){
            scommand_set_redir_out(result, current_arg);
        }
        if (type == ARG_INPUT){
            scommand_set_redir_in(result, current_arg);
        }
        current_arg = parser_next_argument(p, &type);
    }
    if (scommand_is_empty(result)){
        scommand_destroy(result);
        return NULL;
    }
    return result;
}

pipeline parse_pipeline(Parser p) {
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe=true;

    cmd = parse_scommand(p);
    error = (cmd==NULL); /* Comando inválido al empezar */
    while (another_pipe && !error) {
        pipeline_push_back(result,cmd);

        parser_op_pipe(p, &another_pipe);
        cmd = parse_scommand(p);
        error = (cmd == NULL);
    }

    if (!parser_at_eof(p)) {
        bool op_background;
        parser_op_background(p, &op_background);
        pipeline_set_wait(result, !op_background);
    }
    parser_skip_blanks(p); /* Tolerancia a espacios posteriores */
    parser_garbage(p, &error); /* Consumir todo lo que hay inclusive el \n */

    if (error || pipeline_is_empty(result)){
        pipeline_destroy(result);
        return NULL;
    }
    /* Si hubo error, hacemos cleanup */

    return result;
}

