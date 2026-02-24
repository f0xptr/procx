/**
 * @file process_list.c
 * @brief Implementation of process list building and memory management.
 * @version 1.0.0
 */

#include "../../include/system/process_list.h"
#include "../../include/system/sys_info.h"
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

ProcessNode* build_process_list() {
    DIR* dir = opendir("/proc");
    if (!dir) return NULL;

    struct dirent* entry;
    ProcessNode*   head = NULL;
    ProcessNode*   tail = NULL;

    while ((entry = readdir(dir)) != NULL) {
        // If directory name is purely numeric, it's a PID
        if (isdigit(entry->d_name[0])) {
            pid_t pid = (pid_t)atoi(entry->d_name);

            ProcessNode* new_node = (ProcessNode*)malloc(sizeof(ProcessNode));
            if (!new_node) continue;

            if (get_process_info(pid, new_node) == 0) {
                new_node->next = NULL;

                // Append to linked list
                if (!head) {
                    head = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail       = new_node;
                }
            } else {
                free(new_node);  // Failed to read, discard node
            }
        }
    }
    closedir(dir);
    return head;
}

void free_process_list(ProcessNode* head) {
    ProcessNode* current = head;
    while (current != NULL) {
        ProcessNode* next = current->next;
        free(current);
        current = next;
    }
}
