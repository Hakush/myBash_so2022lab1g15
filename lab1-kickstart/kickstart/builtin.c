#include <stdbool.h>
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "strextra.h"
#include "stdio.h"
#include <glib.h>
#include "builtin.h"
#include <assert.h>
#include "tests/syscall_mock.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

void SYS_call_exit_executor(scommand cmd) {
    exit(EXIT_SUCCESS);
}

void SYS_call_help_executor(scommand cmd) {
    char *help = "GNU MyBash \n Autores: Collo Gastón, Rodriguez Diego, Sharry Facundo, Otero Alan \n  cd [-L|[-P [-e]] [-@]] [dir]\t readarray [-d delim] [-n count] [-O origin] [-s count] [-t] [-u fd] [-C callback] [-c quantum] [array]\n";
    char *help2 = " cd [-L|[-P [-e]] [-@]] [dir]\t readarray [-d delim] [-n count] [-O origin] [-s count] [-t] [-u fd] [-C callback] [-c quantum] [array]\n exit [n]\t type [-afptP] name [name ...]\n               help [-dms] [pattern ...]\t { COMMANDS ; }\n";
    char *aux = strmerge(help, help2);
    printf("%s",aux);
}

void SYS_call_cd_executor(scommand cmd) {
    scommand_pop_front(cmd);
    struct passwd *pw = getpwuid(getuid());
    const char * homedir = pw->pw_dir;
    char dir[256] = "";
    if (scommand_is_empty(cmd)){
        strcat(dir, homedir);
    } else {
        char arg[256];
        strcpy(arg, scommand_front(cmd));
        if (!(strncmp(arg, "/", 1) == 0)) {
            if(strncmp(arg, "~", 1) == 0){
                memmove(arg, arg+1, strlen(arg));
                strcat(dir, homedir);
                strcat(dir, arg);
            } else {
                char cwd[256];
                getcwd(cwd, sizeof(cwd));
                strcat(dir, cwd);
                strcat(dir, "/");
                strcat(dir, arg);
            }
        } else {
            strcat(dir, arg);
        }
    }
    puts(dir);
    if(chdir(dir) != 0) {
        perror("chdir() failed");
    }
}

GList * get_sys_commands_list(void) {

    sys_command * c_exit = alloca(sizeof(sys_command));
    c_exit -> name = "exit";
    c_exit -> executor = SYS_call_exit_executor;

    sys_command * c_chdir = alloca(sizeof(sys_command));
    c_chdir -> name = "cd";
    c_chdir -> executor = SYS_call_cd_executor;

    sys_command * c_help = alloca(sizeof(sys_command));
    c_help -> name = "help";
    c_help -> executor = SYS_call_help_executor;

    GList * ret = NULL;
    ret = g_list_append(ret, c_exit);
    ret = g_list_append(ret, c_chdir);
    ret = g_list_append(ret, c_help);
    return ret;
}

bool builtin_is_internal(scommand cmd) {
    assert(cmd != NULL);

    GList * sys_command_list = get_sys_commands_list();
    bool checksum = false;

    while (sys_command_list != NULL && !checksum) {
        sys_command * comando_interno = sys_command_list -> data;
        char * cmd_name = scommand_front(cmd);
        char * builtin_name = (comando_interno -> name);
        checksum = checksum || 0 == strcmp(cmd_name, builtin_name);
        sys_command_list = sys_command_list -> next;
    }

    return checksum;
}

bool builtin_alone(pipeline p) {
    assert(p != NULL);
    return pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p));
}

void builtin_run(scommand cmd) {
    assert(builtin_is_internal(cmd));
    GList * comandos_internos = get_sys_commands_list();
    unsigned int comandos_internos_lenght = g_list_length(comandos_internos) - 1;
    for (unsigned int i = 0; i <= comandos_internos_lenght; ++i) {
        sys_command * comando_interno = g_list_nth_data(comandos_internos, i);
        if (strcmp((comando_interno -> name), scommand_front(cmd)) == 0) {
            comando_interno->executor(cmd);
            break;
        }
    }
}
