#include <stdio.h>
#include "command.h"
#include "builtin.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "execute.h"
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "tests/syscall_mock.h"

void execute_scommand(scommand cmd) {
    assert(cmd!=NULL);
    unsigned int length_cmds = scommand_length(cmd);
    char **list_cmds = calloc(sizeof(char *), length_cmds+1);
    for (unsigned int i = 0; i < length_cmds; ++i) {
        list_cmds[i] = strdup(scommand_front(cmd));
        scommand_pop_front(cmd);
    }
    list_cmds[length_cmds] = NULL;
    int status_code = execvp(list_cmds[0], list_cmds);
    if(status_code < 0) {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}

void execute_single_command(pipeline apipe, scommand cmd, int file_descriptor[2]) {
    assert(cmd != NULL);
    pid_t pid = fork();
    if(pid < 0) {
        perror("Fork failed");
    } else if(pid == 0) {
        set_input(cmd);
        set_output(cmd);
        execute_scommand(cmd);
    } else {
        if (pipeline_get_wait(apipe)) {
            wait(NULL);
        }
    }
}

void execute_pipeline(pipeline apipe) {
    assert(apipe!=NULL);
    if(pipeline_is_empty(apipe)){
        return;
    }
    scommand cmd = pipeline_front(apipe);
    if (builtin_is_internal(cmd)) {
        builtin_run(cmd);
    } else {
        int file_descriptor[2];
        // Caso un solo comando
        if(pipeline_length(apipe) == 1) {
            execute_single_command(apipe, cmd, file_descriptor);
            pipeline_pop_front(apipe);
        } else {
            while (!pipeline_is_empty(apipe) && apipe != NULL) {
                cmd = pipeline_front(apipe);
                if(pipeline_length(apipe) == 1) {
//                    execute_single_command(apipe, cmd, file_descriptor); Linea necesaria para implementar pipelines impares
                } else {
                    execute_multiple_command(apipe, cmd, file_descriptor);
                }
                pipeline_pop_front(apipe);
            }
        }
    }
}

void execute_multiple_command(pipeline apipe, scommand cmd, int file_descriptor[2]) {
    pipe(file_descriptor);
    pid_t first_pid = fork();
        if(first_pid < 0) {
            perror("first multiple fork failed");
        } else if (first_pid==0) {
            printf("im child 1, pid= %d\n",getpid());
            set_input(cmd);
            set_output(cmd);
            close(file_descriptor[0]);
            dup2(file_descriptor[1], 1);
            close(file_descriptor[1]);
            execute_scommand(pipeline_front(apipe));
        } else {
            printf("in parent\n");
            pid_t second_pid = fork();
            if(second_pid < 0) {
                perror("Fork second failed");
                exit(EXIT_FAILURE);
            }
        if(second_pid == 0) {
            printf("im child 2, pid= %d\n",getpid());
            pipeline_pop_front(apipe);
            set_input(cmd);
            set_output(cmd);
            close(file_descriptor[1]);
            dup2(file_descriptor[0], 0);
            close(file_descriptor[0]);
            execute_scommand(pipeline_front(apipe));
        } else {
            close(file_descriptor[0]);
            close(file_descriptor[1]);
            if (pipeline_get_wait(apipe)) {
                wait(NULL);
                wait(NULL);
            }
        }
    }
}

void set_output(scommand cmd) {
    assert(cmd != NULL);
    char *out = scommand_get_redir_out(cmd);
    if(out != NULL) {
        int file_descriptor = open(out, O_CREAT | O_WRONLY, S_IRWXU);
        if(file_descriptor < 0) {
            perror("error open()>output()");
            exit(EXIT_FAILURE);
        }
        dup2(file_descriptor, 1);
        close(file_descriptor);
    }
}

void set_input(scommand cmd) {
    assert(cmd != NULL);
    char *in = scommand_get_redir_in(cmd);
    if(in != NULL) {
        int file_descriptor = open(in, O_RDONLY, S_IRWXU);
        if(file_descriptor < 0) {
            perror("error open()>input()");
            exit(EXIT_FAILURE);
        }
        dup2(file_descriptor, 0);
        close(file_descriptor);
    }
}