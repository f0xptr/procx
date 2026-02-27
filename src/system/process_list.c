/**
 * @file process_list.c
 * @brief Implementation of process list building and memory management.
 * @version 1.1.0
 */

#include "../../include/system/process_list.h"
#include "../../include/system/sys_info.h"
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @file process_list.c
 * @brief Implementation of process list building, sorting, and CPU calculation.
 * @version 1.1.0
 */

#include "../../include/system/process_list.h"
#include "../../include/system/sys_info.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

/**
 * @struct PrevTicks
 * @brief Stores previous tick counts for a process to calculate CPU usage.
 */
typedef struct PrevTicks {
    pid_t pid;
    unsigned long utime;
    unsigned long stime;
} PrevTicks;

static PrevTicks* prev_ticks_list = NULL;
static int prev_ticks_count = 0;

/**
 * @brief Updates or adds a process's ticks to the global tracking list.
 */
static void update_prev_ticks(pid_t pid, unsigned long utime, unsigned long stime) {
    for (int i = 0; i < prev_ticks_count; i++) {
        if (prev_ticks_list[i].pid == pid) {
            prev_ticks_list[i].utime = utime;
            prev_ticks_list[i].stime = stime;
            return;
        }
    }
    prev_ticks_list = realloc(prev_ticks_list, sizeof(PrevTicks) * (prev_ticks_count + 1));
    prev_ticks_list[prev_ticks_count].pid = pid;
    prev_ticks_list[prev_ticks_count].utime = utime;
    prev_ticks_list[prev_ticks_count].stime = stime;
    prev_ticks_count++;
}

/**
 * @brief Retrieves previous ticks for a process.
 */
static int get_prev_ticks(pid_t pid, unsigned long* utime, unsigned long* stime) {
    for (int i = 0; i < prev_ticks_count; i++) {
        if (prev_ticks_list[i].pid == pid) {
            *utime = prev_ticks_list[i].utime;
            *stime = prev_ticks_list[i].stime;
            return 0;
        }
    }
    return -1;
}

ProcessNode* build_process_list() {
    DIR* dir = opendir("/proc");
    if (!dir) return NULL;

    struct dirent* entry;
    ProcessNode*   head = NULL;
    ProcessNode*   tail = NULL;

    static unsigned long long last_total_time = 0;
    unsigned long long total_time = 0;
    FILE* stat_file = fopen("/proc/stat", "r");
    if (stat_file) {
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
        if (fscanf(stat_file, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
                   &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal) == 8) {
            total_time = user + nice + system + idle + iowait + irq + softirq + steal;
        }
        fclose(stat_file);
    }

    unsigned long long total_time_diff = total_time - last_total_time;

    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            pid_t pid = (pid_t)atoi(entry->d_name);
            ProcessNode* new_node = (ProcessNode*)malloc(sizeof(ProcessNode));
            if (!new_node) continue;

            if (get_process_info(pid, new_node) == 0) {
                unsigned long prev_utime, prev_stime;
                if (get_prev_ticks(pid, &prev_utime, &prev_stime) == 0 && total_time_diff > 0) {
                    unsigned long process_diff = (new_node->utime + new_node->stime) - (prev_utime + prev_stime);
                    new_node->cpu_usage = (float)(process_diff * 100.0) / total_time_diff;
                } else {
                    new_node->cpu_usage = 0.0f;
                }
                update_prev_ticks(pid, new_node->utime, new_node->stime);

                new_node->next = NULL;
                if (!head) {
                    head = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail       = new_node;
                }
            } else {
                free(new_node);
            }
        }
    }
    closedir(dir);
    last_total_time = total_time;
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

/**
 * @brief Merges two sorted linked lists.
 */
static ProcessNode* merge(ProcessNode* a, ProcessNode* b, int (*cmp)(ProcessNode*, ProcessNode*)) {
    if (!a) return b;
    if (!b) return a;

    ProcessNode* result = NULL;
    if (cmp(a, b) <= 0) {
        result = a;
        result->next = merge(a->next, b, cmp);
    } else {
        result = b;
        result->next = merge(a, b->next, cmp);
    }
    return result;
}

/**
 * @brief Splits a linked list into two halves.
 */
static void split(ProcessNode* source, ProcessNode** front, ProcessNode** back) {
    ProcessNode* fast;
    ProcessNode* slow;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

/**
 * @brief Sorts a linked list using the Merge Sort algorithm.
 */
void sort_process_list(ProcessNode** head_ref, int (*cmp)(ProcessNode*, ProcessNode*)) {
    ProcessNode* head = *head_ref;
    ProcessNode* a;
    ProcessNode* b;

    if ((head == NULL) || (head->next == NULL)) return;

    split(head, &a, &b);

    sort_process_list(&a, cmp);
    sort_process_list(&b, cmp);

    *head_ref = merge(a, b, cmp);
}

// Comparison functions for sorting
int cmp_pid(ProcessNode* a, ProcessNode* b) { return a->pid - b->pid; }
int cmp_cpu(ProcessNode* a, ProcessNode* b) { return (b->cpu_usage > a->cpu_usage) ? 1 : -1; }
int cmp_mem(ProcessNode* a, ProcessNode* b) { return b->memory_kb - a->memory_kb; }
int cmp_name(ProcessNode* a, ProcessNode* b) { return strcasecmp(a->name, b->name); }

