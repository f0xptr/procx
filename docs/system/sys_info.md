# System: System and Process Information

This module is responsible for gathering detailed system-wide statistics and individual process information from the `/proc` filesystem on Linux.

## `SystemInfo` Struct

The `SystemInfo` struct holds various global system resource usage statistics.

```c
typedef struct SystemInfo {
    int    cpu_usage;     // Total CPU Usage in percentage (0-100)
    int    mem_usage;     // Total RAM Usage in percentage (0-100)
    int    swp_usage;     // Total Swap Usage in percentage (0-100)
    long   total_mem_kb;  // Total RAM in KB
    long   free_mem_kb;   // Free RAM in KB
    long   total_swp_kb;  // Total Swap in KB
    long   free_swp_kb;   // Free Swap in KB
    int    running_tasks; // Number of running processes
    int    total_tasks;   // Number of total processes
    double load_avg[3];   // Load average for 1, 5, and 15 minutes
    long   uptime_sec;    // System uptime in seconds
} SystemInfo;
```

### Functions

### `int get_process_info(pid_t pid, ProcessNode *info)`

*   **Description**: Fetches detailed process information, including PID, name, state, PPID, CPU ticks (utime/stime), priority, nice value, and memory usage (RSS). It reads data from `/proc/[pid]/stat`, `/proc/[pid]/statm`, and `/proc/[pid]/status`.
*   **Parameters**:
    *   `pid`: The Process ID to query.
    *   `info`: Pointer to a `ProcessNode` struct to populate with the retrieved information.
*   **Returns**: `0` on success, `-1` on failure (e.g., process does not exist or cannot be accessed).

### `void get_system_info(SystemInfo *sys_info, ProcessNode *head)`

*   **Description**: Fetches global system resource statistics including CPU usage, memory usage, swap usage, task counts, load averages, and system uptime. It reads data from `/proc/meminfo`, `/proc/stat`, `/proc/loadavg`, and `/proc/uptime`. The CPU usage calculation uses a static approach based on previous readings for estimation.
*   **Parameters**:
    *   `sys_info`: Pointer to a `SystemInfo` struct to populate with system statistics.
    *   `head`: Pointer to the head of the `ProcessNode` linked list, used to count total and running tasks.
*   **Returns**: `void`. The function populates the `sys_info` struct directly.
