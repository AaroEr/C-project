#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define shm_size 1024 // Define shared memory size

int main() {
    // Generate a unique key for shared memory
    key_t key = ftok("sharedmemory", 66);
    if (key == -1) {
        exit(1);
    }

     // Get the shared memory segment
    int shm_id = shmget(key, shm_size, 0666);
    if (shm_id == -1) {
        exit(1);
    }

    // Attach the shared memory segment to the process
    int *shared_memory = (int*) shmat(shm_id, NULL, 0);
    if (shared_memory == (int*)(-1)) {
        exit(1);
    }

    pid_t processIDs[4]; // Array to store process IDs
    int values[4]={5, 12, 7, 19}; // Values to be written by child processes
    int pipes[4][2]; // Pipes for communication
    int read_value; // Variable to store read values

     // Create pipes for each child process
    for (int i = 0; i < 4; i++) {
        if (pipe(pipes[i]) == -1) {
            exit(EXIT_FAILURE);
        }
    }

    // Create 4 child processes
    for (int i = 0; i < 4; i++) {
        processIDs[i] = fork();

        if (processIDs[i] < 0) {
            exit(EXIT_FAILURE);
        } else if (processIDs[i] == 0) {
            close(pipes[i][0]);
            write(pipes[i][1], &values[i], sizeof(values[i]));
            printf("Written value: %d\n", values[i]);
            close(pipes[i][1]);
            printf("Process ID: %d\n", getpid());
            exit(EXIT_SUCCESS);
        }
    }

     // Close write ends of all pipes in the parent process
    for (int i = 0; i < 4; i++) {
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 4; i++) {
        waitpid(processIDs[i], NULL, 0);
    }

    // Read values from pipes and store them in shared memory
    for (int i = 0; i < 4; i++) {
        read(pipes[i][0], &read_value, sizeof(read_value));
        shared_memory[i] = read_value;
        close(pipes[i][0]);
    }

    // Detach from shared memor
    shmdt(shared_memory);

    return 0;
}