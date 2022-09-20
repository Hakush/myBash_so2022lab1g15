#ifndef _BUILTIN_H_
#define _BUILTIN_H_

#include <stdbool.h>

#include "command.h"
#include <glib.h>

/*
 *  Estructura que guarda por un lado en un string el nombre del comando y por el otro la llamada a la funcion al comando
 */
typedef struct sys_calls_command {
    char * name;
    void (*executor)(scommand cmd);
} sys_command;

bool builtin_is_internal(scommand cmd);
/*
 * Indica si el comando alojado en `cmd` es un comando interno
 *
 * REQUIRES: cmd != NULL
 *
 */

GList * get_sys_commands_list(void);
/*
 Coloca los comandos internos en una GLIST de tipo sys_command de comandos internos, con sus respectivos nombres y syscalls
 */

void SYS_call_exit_executor(scommand cmd);
/*
    Ejecuta el comando exit, que se encarga de salir ded mybash
 */

void SYS_call_help_executor(scommand cmd);
/*
    Ejecuta el comando help, que imprime por pantalla una ayuda sobre que y como escribir comandos en mybash
 */

void SYS_call_cd_executor(scommand cmd);
/*
    Ejecuta el comando cd, que nos deja navegar entre directorios ded nuestro sistema.
 */

bool builtin_alone(pipeline p);
/*
 * Indica si el pipeline tiene solo un elemento y si este se corresponde a un
 * comando interno.
 *
 * REQUIRES: p != NULL
 *
 * ENSURES:
 *
 * builtin_alone(p) == pipeline_length(p) == 1 &&
 *                     builtin_is_internal(pipeline_front(p))
 *
 *
 */

void builtin_run(scommand cmd);
/*
 * Ejecuta un comando interno
 *
 * REQUIRES: {builtin_is_internal(cmd)}
 *
 */

#endif

