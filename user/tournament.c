#include "kernel/types.h"
#include "user/user.h"



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        exit(1);
    }
    int num_processes = atoi(argv[1]);
    int result = tournament_create(num_processes);
    if (result < 0 && result != -2) {
        printf("Failed to create tournament tree\n");
        exit(1);
    }

    if (result >= 0) {
        if (tournament_acquire() < 0) {
            printf("Failed to acquire tournament lock\n");
            exit(1);
        }

        printf("Process %d (index %d) in critical section\n", getpid(), result);

        if (tournament_release() < 0) {
            printf("Failed to release tournament lock\n");
            exit(1);
        }

        exit(0); // child exits
    }

    for (int i = 0; i < num_processes; i++) {
        wait(0);
    }

    printf("Parent process: all children completed\n");
    exit(0);
}