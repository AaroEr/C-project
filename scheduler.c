#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define shm_size 1024 // Define shared memory size

// Comparison function for qsort (ascending order)
int compare(const void *a, const void *b) {
    return(*(int*)a-*(int*)b);
}

int main() {
    int values[4]; // Array to store retrieved values

    // Generate the same key to access shared memory
    key_t key = ftok("sharedmemory", 66);
    if (key == -1) {
        exit(1);
    }

    // Access the shared memory segment
    int shm_id = shmget(key, shm_size, 0666 | IPC_CREAT);
    if (shm_id == -1) {
        exit(1);
    }

    // Attach the shared memory segment
    int *shared_memory = (int*) shmat(shm_id, NULL, 0);
    if (shared_memory == (int*)(-1)) {
        exit(1);
    }

    // Wait for user input to ensure init.c has finished writing
    getchar();

    // Read values from shared memory into the array
    for (int i = 0; i < 4; i++) {
        values[i] = shared_memory[i];
    }

    // Detach from shared memory
    shmdt(shared_memory);

    // Remove shared memory segment
    shmctl(shm_id, IPC_RMID, NULL);

    // Sort values in ascending order
    qsort(values, 4, sizeof(int), compare);

    // Print sorted values
    for (int i = 0; i < 4; i++) {
        printf("%d\n", values[i]);
    }

    return 0;
}