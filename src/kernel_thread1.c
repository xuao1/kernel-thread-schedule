#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <pid1> <pid2>\n", argv[0]);
        return 1;
    }

    int pid1 = atoi(argv[1]);
    int pid2 = atoi(argv[2]);

    while (1) {
        printf("Sending SIGSTOP to %d and %d\n", pid1, pid2);
        kill(pid1, SIGSTOP);
        kill(pid2, SIGSTOP);
        sleep(5);

        printf("Sending SIGCONT to %d and %d\n", pid1, pid2);
        kill(pid1, SIGCONT);
        kill(pid2, SIGCONT);
        sleep(5);
    }

    return 0;
}
