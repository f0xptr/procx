/**
 * @file test_sys_info.c
 * @brief Unit tests for system information parsing.
 * @version 1.1.1
 */

#include "../include/system/sys_info.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief Tests the get_process_info function by querying the current process.
 *
 * Verifies that the PID, PPID, UID, number of threads, and username
 * are correctly retrieved and populated for the running test process.
 */
void test_current_process_parsing() {
    ProcessNode info;
    pid_t       my_pid = getpid();

    int result = get_process_info(my_pid, &info);

    assert(result == 0);
    assert(info.pid == my_pid);
    assert(info.ppid == getppid());
    assert(info.uid == getuid());
    assert(info.num_threads > 0);
    assert(info.username[0] != '\0');  // Ensure username is not empty
    printf("OK: get_process_info() successfully parsed PID %d (User: %s, Threads: %d)\n", my_pid,
           info.username, info.num_threads);
}

/**
 * @brief Main entry point for the test suite.
 * Executes all defined unit tests for system information parsing.
 * @return int Returns 0 if all tests pass, non-zero otherwise.
 */
int main() {
    printf("Running ProcX Unit Tests...\n");
    test_current_process_parsing();
    printf("All tests passed!\n");
    return 0;
}
