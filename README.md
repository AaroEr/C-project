# Inter-Process Communication Using Shared Memory and Pipes

## Overview

This project implements inter-process communication (IPC) between two processes: `init` and `scheduler`. The `init` process creates multiple child processes (P1-P4),
which sends hard coded value to `init` using pipes. The `scheduler` process retrieves these values from shared memory, sorts them, and prints the sorted list.

## Implementation Details

### Processes & Communication

#### `init.c`:

- Forks four child processes (P1-P4).
- Each child sends hard coded integer and sends it to `init` via pipes.
- Reads values from pipes and writes them to shared memory.

#### `scheduler.c`:

- Creates a shared memory segment.
- Waits for `init` to write data, then reads it.
- Sorts the numbers in ascending order and prints the result.
- Cleans up shared memory upon completion.

### System Calls Used

- **Process Creation:** `fork()`
- **Inter-Process Communication:** `pipe()`, `read()`, `write()`
- **Shared Memory Management:** `shmget()`, `shmat()`, `shmdt()`, `shmctl()`

## Key Learning Outcomes

- Understanding process creation and management using `fork()`.
- Implementing pipes for message passing.
- Managing shared memory for inter-process communication.
- Applying sorting algorithms in IPC scenarios.
