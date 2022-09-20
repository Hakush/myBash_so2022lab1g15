#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <sys/wait.h>

static void show_prompt(void) {
    struct passwd *pw = getpwuid(getuid());

    char cwd[256];
    getcwd(cwd, sizeof(cwd));

    FILE * hostnamefd = fopen("/proc/sys/kernel/hostname", "r");
    char hostname[256];
    fgets(hostname, 256, hostnamefd);
    hostname[strcspn(hostname, "\r\n")] = 0; // Elimina el \n o \r

    printf ("%s@%s:%s$ ",(pw->pw_name), hostname,  cwd);
    fflush (stdout);
}

int main(int argc, char *argv[]) {
    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    while (!quit) {
        show_prompt();
        while(waitpid(-1, NULL, WNOHANG) > 0);
        pipe = parse_pipeline(input);

        quit = parser_at_eof(input);
        /* Hay que salir luego de ejecutar? */
        if(pipe != NULL) {
            execute_pipeline(pipe);
        }
    }
    parser_destroy(input);
    input = NULL;
    return EXIT_SUCCESS;
}

