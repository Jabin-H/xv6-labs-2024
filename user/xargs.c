#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int readline(char *argv[MAXARG], int argc) {
    int i = 0, new_argc = argc;
    char buf[512], ch;
    while (read(0, &ch, 1)) {
        if (ch == ' ' || ch == '\n') {
            buf[i] = '\0';
            argv[new_argc] = malloc(i);
            strcpy(argv[new_argc], buf);
            new_argc++;
            i = 0;
            if (ch == ' ') {
                continue;
            } else {
                break;
            }
        }
        buf[i++] = ch;
    }
    if (new_argc == argc) return 0;
    return new_argc;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(2, "Usage: xargs command (arg ..)\n");
    }
    char *xargv[MAXARG];
    int i, new_argc;
    for (i = 0; i < argc - 1; i++) {
        xargv[i] = malloc(strlen(argv[i + 1]) + 1);
        strcpy(xargv[i], argv[i + 1]);
    }
    while ((new_argc = readline(xargv, argc - 1)) != 0) {
        xargv[new_argc + 1] = 0;
        if (fork() == 0) {
            exec(xargv[0], xargv);
            fprintf(2, "exec failed\n");
            exit(1);
        } else {
            wait(0);
        }
    }
    exit(0);
}