/**
 * @file sys_info.h
 * @brief System information gathering functions.
 * @version 1.1.0
 */

#ifndef PROCX_SYS_INFO_H
#define PROCX_SYS_INFO_H

#include "../core/process.h"

/**
 * @struct SystemInfo
 * @brief Global system resource usage statistics.
 */
typedef struct SystemInfo {
    int   cpu_usage;     /**< Total CPU Usage in percentage (0-100) */
    int   mem_usage;     /**< Total RAM Usage in percentage (0-100) */
    int   swp_usage;     /**< Total Swap Usage in percentage (0-100) */
    long  total_mem_kb;  /**< Total RAM in KB */
    long  free_mem_kb;   /**< Free RAM in KB */
    long  total_swp_kb;  /**< Total Swap in KB */
    long  free_swp_kb;   /**< Free Swap in KB */
    int   running_tasks; /**< Number of running processes */
    int   total_tasks;   /**< Number of total processes */
    double load_avg[3];  /**< Load average for 1, 5, and 15 minutes */
    long   uptime_sec;   /**< System uptime in seconds */
} SystemInfo;

/**
 * @brief Fetches basic process information and memory usage for a specific PID.
 * @param pid The Process ID to query.
 * @param info Pointer to ProcessNode struct to populate.
 * @return int 0 on success, -1 on failure.
 */
int get_process_info(pid_t pid, ProcessNode* info);

/**
 * @brief Fetches global system resource statistics (CPU, Mem, Swap, Tasks).
 * @param sys_info Pointer to SystemInfo struct to populate.
 */
void get_system_info(SystemInfo* sys_info, ProcessNode* head);

#endif  // PROCX_SYS_INFO_H
