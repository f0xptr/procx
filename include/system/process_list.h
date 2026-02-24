/**
 * @file process_list.h
 * @brief Functions to scan and build a list of all running processes.
 * @version 1.0.0
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

#endif  // PROCX_PROCESS_LIST_H
