/**
 * @file process.h
 * @brief Core data structures for ProcX.
 * @version 1.0.0
 */

#ifndef PROCX_PROCESS_H
#define PROCX_PROCESS_H

#include <sys/types.h>

/**
 * @struct ProcessNode
 * @brief Linked list node representing a single system process.
 */
typedef struct ProcessNode {
    pid_t               pid;          /**< Process ID */
    pid_t               ppid;         /**< Parent Process ID */
    uid_t               uid;          /**< User ID */
    char                username[32]; /**< Username */
    int                 num_threads;  /**< Number of threads */
    char                name[256];    /**< Name of the process executable */
    char                state;        /**< Process state (e.g., R, S, Z) */
    long                memory_kb;    /**< Resident Set Size (RAM used) in KB */
    struct ProcessNode* next;         /**< Pointer to the next process in the list */
} ProcessNode;

#endif  // PROCX_PROCESS_H
