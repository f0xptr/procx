# Core: Process Data Structures

This document describes the core data structures used in ProcX to represent processes and their information.

## `ProcessNode` Struct

The `ProcessNode` is a fundamental building block for the process list within ProcX. It represents a single process running on the system and is designed to be part of a linked list.

```c
typedef struct ProcessNode {
    pid_t pid;                  // Process ID
    pid_t ppid;                 // Parent Process ID
    uid_t uid;                  // User ID
    char username[32];          // Username
    int num_threads;            // Number of threads
    char name[256];             // Name of the process executable
    char state;                 // Process state (e.g., R, S, Z)
    long memory_kb;             // Resident Set Size (RAM used) in KB
    struct ProcessNode *next;   // Pointer to the next process in the list
} ProcessNode;
```

### Members

*   `pid`: The unique process identifier.
*   `ppid`: The process ID of the parent process.
*   `uid`: The user ID of the process owner.
*   `username[32]`: A null-terminated string storing the username of the process owner.
*   `num_threads`: The number of threads associated with the process.
*   `name[256]`: A null-terminated string storing the name of the executable.
*   `state`: A character representing the current state of the process (e.g., 'R' for running, 'S' for sleeping, 'Z' for zombie).
*   `memory_kb`: The Resident Set Size (RSS) of the process, indicating the amount of RAM it is currently using, in kilobytes.
*   `next`: A pointer to the next `ProcessNode` in the linked list, or `NULL` if it is the last node.

### Usage

Instances of `ProcessNode` are dynamically allocated and linked together to form a list representing all active processes. Functions in the `system/process_list` module are responsible for creating, populating, and freeing these nodes.
