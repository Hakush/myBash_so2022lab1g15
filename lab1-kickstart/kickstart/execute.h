/* Ejecuta comandos simples y pipelines.
 * No toca ningún comando interno.
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "command.h"

void execute_scommand(scommand cmd);
/*
 * Ejecuta un comando simple
 */

void set_output(scommand cmd);
/*
 Setea el output de cmd para reemplazarlo en la process table por el STDOUT
 */

void set_input(scommand cmd);
/*
 Setea el input de cmd para reemplazarlo en la process table por el STDIN
 */

void execute_pipeline(pipeline apipe);
/*
 * Ejecuta un pipeline, identificando comandos internos, forkeando, y
 *   redirigiendo la entrada y salida. puede modificar `apipe' en el proceso
 *   de ejecución.
 *   apipe: pipeline a ejecutar
 * Requires: apipe!=NULL
 */
void execute_multiple_command(pipeline apipe, scommand cmd, int file_descriptor[2]);
/*
 * Ejecuta multiples comandos de un pipeline, tomando de a pares y redireccionando los outputs hacia los inputs correspondientes
 */
void execute_single_command(pipeline apipe, scommand cmd, int file_descriptor[2]);
/*
 * Ejecuta un comando solo de un pipeline
 */

#endif /* EXECUTE_H */
