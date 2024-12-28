#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p[2]) __attribute__((noreturn));

void primes(int p[2]) {
    int prime, n;
    close(p[1]);
    if (read(p[0], &prime, 4) != 4) {
        fprintf(2, "failed to read\n");
        exit(1);
    }
    printf("prime %d\n", prime);
    if (read(p[0], &n, 4)) {
        int new_p[2];
        pipe(new_p);
        if (fork() == 0) {
            primes(new_p);
        } else {
            close(new_p[0]);
            do {
                if (n % prime != 0)
                    write(new_p[1], &n, 4);
            } while (read(p[0], &n, 4));
            close(p[0]);
            close(new_p[1]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        primes(p);
    } else {
        close(p[0]);
        for (int i = 2; i <= 280; i++) {
            if (write(p[1], &i, 4) != 4) {
                fprintf(2, "failed to write %d into pipe", i);
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}