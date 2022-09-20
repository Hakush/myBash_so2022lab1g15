#include <stdbool.h> /* para tener bool */
#include <string.h>
#include <assert.h> /* necesito para las afirmaciones */
#include <stdlib.h>
#include <glib.h>
#include "command.h"
#include "strextra.h"
#include <check.h>

scommand scommand_new(void) {
      scommand result = malloc(sizeof (scommand));
      result -> input = NULL;
      result -> output = NULL;
      result -> commandList = NULL;

      assert(result != NULL
            && scommand_is_empty(result)
            && scommand_get_redir_in (result) == NULL
            && scommand_get_redir_out (result) == NULL);
      return result;
}

scommand scommand_destroy(scommand self) {
    assert(self != NULL);
    self = NULL;
    return self;
}


void scommand_push_back(scommand self, char * argument) {
    assert(self!=NULL && argument!=NULL);
    self -> commandList = g_list_append (self -> commandList, argument); /* método "append" en GLib>List>append */
    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self) {
    assert(self!=NULL && !scommand_is_empty(self));
    gpointer first = g_list_nth_data(self -> commandList, 0); /*Obtengo el 1er elemento de la lista GLib>List>nth_data*/
    self -> commandList = g_list_remove(self -> commandList, first); /*Elimino el 1er elemento GLib>List>remove*/
    first = NULL;
}

void scommand_set_redir_in(scommand self, char * filename) {
    assert(self != NULL);
    self -> input = filename;
}
void scommand_set_redir_out(scommand self, char * filename) {
    assert(self != NULL);
    self -> output = filename;
}

bool scommand_is_empty(const scommand self) {
    assert(self != NULL);
    return self -> commandList != NULL ? false : true;
}

unsigned int scommand_length(const scommand self) {
    assert(self != NULL);
    return g_list_length(self -> commandList);
}

char * scommand_front(const scommand self) {
    assert(self != NULL && !scommand_is_empty(self));
    return (g_list_first(self -> commandList) -> data);
}

char * scommand_get_redir_in(const scommand self) {
    assert(self != NULL);
    return self -> input;
}
char * scommand_get_redir_out(const scommand self) {
    assert(self != NULL);
    return self -> output;
}


char * scommand_to_string(const scommand self) {
    assert(self != NULL);
    char * ret;
    ret = malloc(1024 * sizeof(char*));
    *ret = '\0';
    GList * elem = self -> commandList;
    while ( elem != NULL) {
        ret = strmerge( ret, (elem -> data));
        elem = elem -> next;
    }

    if ((self -> output)!= NULL)
    {
        strcat(ret, " > ");
        strcat(ret, self -> output);

    }

    if ((self -> input)!= NULL)
    {
        strcat(ret, " < ");
        strcat(ret, self -> input);
    }

    assert(scommand_is_empty(self)
           || scommand_get_redir_in(self)==NULL
           || scommand_get_redir_out(self)==NULL
           || strlen(ret)>0);

    return ret;
}

struct pipeline_s {
    GList *command;
    bool wait;
};

pipeline pipeline_new(void) {
    pipeline result = malloc(sizeof(pipeline));
    result -> command = NULL;
    result -> wait = true;
    assert(result!=NULL
        && pipeline_is_empty(result)
        && pipeline_get_wait(result));
    return result;
}

pipeline pipeline_destroy(pipeline self){
    assert(self != NULL);
    self = NULL;
    assert(self == NULL);
    return self;
}

/* Modificadores */

void pipeline_push_back(pipeline self, scommand argument) {
    assert(self!=NULL && argument!=NULL);
    self -> command = g_list_append (self -> command, argument);
}

void pipeline_pop_front(pipeline self) {
    assert(self!=NULL && !pipeline_is_empty(self));
    scommand first = g_list_nth_data(self -> command, 0);
    (self -> command) = g_list_remove(self -> command, first);
    scommand_destroy(first);
}

void pipeline_set_wait(pipeline self, const bool w) {
    assert(self!=NULL);
    self -> wait = (bool) w;
}

/* Proyectores */

bool pipeline_is_empty(const pipeline self) {
    assert(self != NULL);
    return self -> command != NULL ? false : true;
}

unsigned int pipeline_length(const pipeline self){
    assert(self != NULL);
    guint ret = g_list_length(self -> command);
    assert((ret == 0) == pipeline_is_empty(self));
    return ret;
}

scommand pipeline_front(const pipeline self){
    assert(self != NULL && !pipeline_is_empty(self));
    scommand first = (scommand) g_list_nth_data(self -> command, 0);
    assert(first != NULL);
    return first;
}

bool pipeline_get_wait(const pipeline self) {
    assert(self != NULL);
    return self -> wait;
}

char * pipeline_to_string(const pipeline self){
    assert(self != NULL);

    char * ret;
    ret = malloc(1024 * sizeof(char*));
    *ret = '\0';
    GList * scommandslist = self -> command;

    while (scommandslist != NULL) {
        scommand command = scommandslist -> data;
        ret = strmerge(ret, scommand_to_string(command));
        scommandslist = scommandslist -> next;
        if (scommandslist != NULL) {
            ret = strmerge(ret, "|");
        } else {
            ret = strmerge(ret, " & ");
        }
    }
    assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(ret)>0);
    return ret;
}
