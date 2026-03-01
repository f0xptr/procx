/**
 * @file sys_info.c
 * @brief Implementation of system info parsing from /proc.
 * @version 1.1.1
 */

#include "../../include/system/sys_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

int get_process_info(pid_t pid, ProcessNode* info) {
    char  path[256];
    FILE* file;

    // Get basic process information (name, state, ppid, utime, stime)
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    file = fopen(path, "r");
    if (!file) return -1;

    info->pid = pid;
    // Format: pid (comm) state ppid pgrp session tty_nr tpgid flags minflt cminflt majflt cmajflt
    // utime stime ... utime is 14th field, stime is 15th field
    if (fscanf(file, "%*d (%255[^)]) %c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", info->name,
               &info->state, &info->ppid, &info->utime, &info->stime) < 5) {
        fclose(file);
        return -1;
    }
    fclose(file);

    // Get memory information (Resident Set Size)
    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    file = fopen(path, "r");
    if (file) {
        long dummy, rss;
        if (fscanf(file, "%ld %ld", &dummy, &rss) == 2) {
            long page_size  = sysconf(_SC_PAGESIZE) / 1024;
            info->memory_kb = rss * page_size;
        } else {
            info->memory_kb = 0;
        }
        fclose(file);
    } else {
        info->memory_kb = 0;
    }

    // Get UID and Threads from status file
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    file = fopen(path, "r");
    if (file) {
        char line[256];
        info->uid         = 0;
        info->num_threads = 1;
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "Uid:", 4) == 0) {
                sscanf(line, "Uid:\t%u", &info->uid);
            } else if (strncmp(line, "Threads:", 8) == 0) {
                sscanf(line, "Threads:\t%d", &info->num_threads);
            }
        }
        fclose(file);

        struct passwd* pw = getpwuid(info->uid);
        if (pw) {
            strncpy(info->username, pw->pw_name, sizeof(info->username) - 1);
            info->username[sizeof(info->username) - 1] = '\0';
        } else {
            snprintf(info->username, sizeof(info->username), "%u", info->uid);
        }
    } else {
        info->uid         = 0;
        info->num_threads = 1;
        strcpy(info->username, "unknown");
    }

    return 0;
}

void get_system_info(SystemInfo* sys_info, ProcessNode* head) {
    FILE* file;
    char  line[256];

    // Initialize defaults
    sys_info->cpu_usage     = 0;
    sys_info->mem_usage     = 0;
    sys_info->swp_usage     = 0;
    sys_info->total_mem_kb  = 0;
    sys_info->free_mem_kb   = 0;
    sys_info->total_swp_kb  = 0;
    sys_info->free_swp_kb   = 0;
    sys_info->running_tasks = 0;
    sys_info->total_tasks   = 0;
    sys_info->uptime_sec    = 0;
    for (int i = 0; i < 3; i++) sys_info->load_avg[i] = 0.0;

    // Parse Load Average
    if (getloadavg(sys_info->load_avg, 3) == -1) {
        // Fallback to /proc/loadavg if getloadavg fails
        file = fopen("/proc/loadavg", "r");
        if (file) {
            fscanf(file, "%lf %lf %lf", &sys_info->load_avg[0], &sys_info->load_avg[1],
                   &sys_info->load_avg[2]);
            fclose(file);
        }
    }

    // Parse Uptime
    file = fopen("/proc/uptime", "r");
    if (file) {
        double uptime;
        if (fscanf(file, "%lf", &uptime) == 1) {
            sys_info->uptime_sec = (long)uptime;
        }
        fclose(file);
    }

    // Parse MemInfo
    file = fopen("/proc/meminfo", "r");
    if (file) {
        long buffers = 0, cached = 0, mem_available = 0;
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "MemTotal:", 9) == 0) {
                sscanf(line, "MemTotal: %ld kB", &sys_info->total_mem_kb);
            } else if (strncmp(line, "MemFree:", 8) == 0) {
                sscanf(line, "MemFree: %ld kB", &sys_info->free_mem_kb);
            } else if (strncmp(line, "MemAvailable:", 13) == 0) {
                sscanf(line, "MemAvailable: %ld kB", &mem_available);
            } else if (strncmp(line, "Buffers:", 8) == 0) {
                sscanf(line, "Buffers: %ld kB", &buffers);
            } else if (strncmp(line, "Cached:", 7) == 0) {
                sscanf(line, "Cached: %ld kB", &cached);
            } else if (strncmp(line, "SwapTotal:", 10) == 0) {
                sscanf(line, "SwapTotal: %ld kB", &sys_info->total_swp_kb);
            } else if (strncmp(line, "SwapFree:", 9) == 0) {
                sscanf(line, "SwapFree: %ld kB", &sys_info->free_swp_kb);
            }
        }
        fclose(file);

        long used_mem = sys_info->total_mem_kb - mem_available;
        if (mem_available == 0) {
            used_mem = sys_info->total_mem_kb - sys_info->free_mem_kb - buffers - cached;
        }

        if (sys_info->total_mem_kb > 0) {
            sys_info->mem_usage = (int)((used_mem * 100) / sys_info->total_mem_kb);
        }

        long used_swp = sys_info->total_swp_kb - sys_info->free_swp_kb;
        if (sys_info->total_swp_kb > 0) {
            sys_info->swp_usage = (int)((used_swp * 100) / sys_info->total_swp_kb);
        }
    }

    // Global CPU usage
    static unsigned long long prev_user = 0, prev_nice = 0, prev_system = 0, prev_idle = 0;
    static unsigned long long prev_iowait = 0, prev_irq = 0, prev_softirq = 0, prev_steal = 0;
    file = fopen("/proc/stat", "r");
    if (file) {
        if (fgets(line, sizeof(line), file)) {
            unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
            if (sscanf(line, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system,
                       &idle, &iowait, &irq, &softirq, &steal) == 8) {
                unsigned long long prev_idle_sum = prev_idle + prev_iowait;
                unsigned long long idle_sum      = idle + iowait;
                unsigned long long prev_non_idle =
                    prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
                unsigned long long non_idle   = user + nice + system + irq + softirq + steal;
                unsigned long long prev_total = prev_idle_sum + prev_non_idle;
                unsigned long long total      = idle_sum + non_idle;
                unsigned long long totald     = total - prev_total;
                unsigned long long idled      = idle_sum - prev_idle_sum;

                if (totald > 0) {
                    sys_info->cpu_usage = (int)(100.0 * (double)(totald - idled) / (double)totald);
                }

                prev_user    = user;
                prev_nice    = nice;
                prev_system  = system;
                prev_idle    = idle;
                prev_iowait  = iowait;
                prev_irq     = irq;
                prev_softirq = softirq;
                prev_steal   = steal;
            }
        }
        fclose(file);
    }

    // Count tasks
    ProcessNode* curr = head;
    while (curr) {
        sys_info->total_tasks++;
        if (curr->state == 'R') sys_info->running_tasks++;
        curr = curr->next;
    }
}
