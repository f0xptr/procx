/**
 * @file process_list.h
 * @brief Functions to scan, build, and sort the list of all running processes.
 * @version 2.0.0
 */

#ifndef PROCX_PROCESS_LIST_H
#define PROCX_PROCESS_LIST_H

#include "../core/process.h"

/**
 * @brief Scans the /proc directory and builds a linked list of processes.
 * @return Pointer to the head of the ProcessNode linked list.
 */
ProcessNode* build_process_list();

/**
 * @brief Frees the memory allocated for the process linked list.
 * @param head Pointer to the head of the list.
 */
void free_process_list(ProcessNode* head);

/**
 * @brief Sorts the process list using a custom comparison function.
 * @param head_ref Pointer to the pointer of the head node.
 * @param cmp Comparison function to use for sorting.
 */
void sort_process_list(ProcessNode** head_ref, int (*cmp)(ProcessNode*, ProcessNode*));

/**
 * @brief Comparison function for PIDs.
 */
int cmp_pid(ProcessNode* a, ProcessNode* b);

/**
 * @brief Comparison function for CPU usage.
 */
int cmp_cpu(ProcessNode* a, ProcessNode* b);

/**
 * @brief Comparison function for Memory usage.
 */
int cmp_mem(ProcessNode* a, ProcessNode* b);

/**
 * @brief Comparison function for Process names.
 */
int cmp_name(ProcessNode* a, ProcessNode* b);

#endif  // PROCX_PROCESS_LIST_H
