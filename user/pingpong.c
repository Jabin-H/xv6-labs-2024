#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid, p[2];
    pipe(p); // read/write fd in p[0], p[1]

    pid = fork();
    if (pid == 0) {
        char buf[1];
        if (read(p[0], buf, 1) != 1) {
            fprintf(2, "failed to read in child\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: received ping\n", getpid());
        if (write(p[1], buf, 1) != 1) {
            fprintf(2, "failede to write in child\n");
            exit(1);
        }
        close(p[1]);
    } else {
        char info = 'a';
        char buf[1];
        if (write(p[1], &info, 1) != 1) {
            fprintf(2, "failed to write in parent\n");
            exit(1);
        }
        close(p[1]);
        wait((int *)0);
        if (read(p[0], buf, 1) != 1) {
            fprintf(2, "failed to read in parent\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}