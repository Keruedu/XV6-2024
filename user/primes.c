#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int left_pipe[2]) __attribute__((noreturn));

void primes(int left_pipe[2]) {
    int p, num;
    int right_pipe[2];
    close(left_pipe[1]);

    if (read(left_pipe[0], &p, sizeof(p)) == 0) {
        close(left_pipe[0]);
        exit(0);
    }
    printf("prime %d\n", p);

    if (pipe(right_pipe) < 0) {
        printf("pipe failed\n");
        exit(1);
    }

    if (fork() == 0) {
        close(left_pipe[0]); 
        primes(right_pipe);   
    } else {
        close(right_pipe[0]); 
        while (read(left_pipe[0], &num, sizeof(num)) > 0) {
           
            if (num % p != 0) {
                write(right_pipe[1], &num, sizeof(num));
            }
        }
        close(left_pipe[0]);  
        close(right_pipe[1]);
        wait(0);              
        exit(0);
    }
}

int main() {
    int i;
    int left_pipe[2];

    if (pipe(left_pipe) < 0) {
        printf("pipe failed\n");
        exit(1);
    }

    if (fork() == 0) {
        primes(left_pipe);
    } else {

        close(left_pipe[0]);
        for (i = 2; i <= 280; i++) {
            if (write(left_pipe[1], &i, sizeof(i)) < 0) {
                printf("write failed\n");
                exit(1);
            }
        }
        close(left_pipe[1]); 
        wait(0);           
        exit(0);
    }
}
