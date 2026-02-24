# System: Process List Management

This module provides functions to scan the system's `/proc` directory, build a linked list of active processes, and manage their memory.

### Functions

### `ProcessNode* build_process_list()`

*   **Description**: Scans the `/proc` directory, identifies process directories (numeric PIDs), and for each valid process, calls `get_process_info` to populate a `ProcessNode`. These nodes are then linked together to form a comprehensive list of all running processes.
*   **Parameters**: None.
*   **Returns**: A pointer to the head of the newly created `ProcessNode` linked list. Returns `NULL` if `/proc` cannot be opened or if no processes are found. It's the caller's responsibility to free this list using `free_process_list`.

### `void free_process_list(ProcessNode *head)`

*   **Description**: Iterates through a `ProcessNode` linked list, starting from the given `head`, and frees all dynamically allocated `ProcessNode`s to prevent memory leaks.
*   **Parameters**:
    *   `head`: A pointer to the head of the `ProcessNode` linked list to be freed.
*   **Returns**: `void`.
