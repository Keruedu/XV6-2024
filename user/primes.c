#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int left_pipe[2]) __attribute__((noreturn));

void primes(int left_pipe[2]) {
    int prime, num;
    close(left_pipe[1]);
    if (read(left_pipe[0], &prime, sizeof(prime)) == 0) {
        // No more numbers to read, end process
        close(left_pipe[0]);
        exit(0);
    }

    printf("prime %d\n", prime);
    int right_pipe[2];
    pipe(right_pipe);
       
    if (fork() == 0) {
        close(left_pipe[0]);
        primes(right_pipe);
    } else {
        close(right_pipe[0]);

        while (read(left_pipe[0], &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
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
    int pipe_main[2];
    pipe(pipe_main);

    if (fork() == 0) {
        primes(pipe_main); 
    } else {
        // In the parent process, generate numbers from 2 to 280 and feed them into the pipeline
        close(pipe_main[0]);
        
        for (int i = 2; i <= 280; i++) {
            write(pipe_main[1], &i, sizeof(i));
        }

        close(pipe_main[1]);
        wait(0);
        exit(0);
    }
}
