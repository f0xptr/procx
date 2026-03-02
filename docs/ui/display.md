# UI: Ncurses Display Functions

This module handles all user interface rendering and interaction using the ncurses library, providing a futuristic "Cyber-Dark" terminal-based dashboard for ProcX.

### Functions

### `void init_ui()`

*   **Description**: Initializes the ncurses environment with wide-character support. This includes setting the locale, setting up the screen, enabling color support, configuring input modes (cbreak, noecho), hiding the cursor, enabling special keys, and defining high-contrast neon color pairs.
*   **Parameters**: None.
*   **Returns**: `void`.

### `void render_dashboard(ProcessNode *head, int scroll_offset, int selection_idx, const char* search_query, const char* sort_col)`

*   **Description**: Clears the screen and renders the main ProcX dashboard. This includes futuristic resource meters, integrated system metrics (tasks, load, uptime), a color-coded process table with descriptive status labels, and a stylized "command center" footer.
*   **Parameters**:
    *   `head`: A pointer to the head of the `ProcessNode` linked list.
    *   `scroll_offset`: Number of processes to skip for scrolling.
    *   `selection_idx`: Index of the currently highlighted process.
    *   `search_query`: Current filter string applied to process names.
    *   `sort_col`: Name of the column currently used for sorting.
*   **Returns**: `void`.

### `void render_process_details(ProcessNode* proc)`

*   **Description**: Renders a dedicated "Process Inspector" popup window showing exhaustive metadata for a specific process, including UID, PPID, exact memory in KB, and CPU time ticks.
*   **Parameters**:
    *   `proc`: Pointer to the `ProcessNode` to inspect.
*   **Returns**: `void`.

### `void close_ui()`

*   **Description**: Cleans up the ncurses environment, restoring the terminal to its original state.
*   **Parameters**: None.
*   **Returns**: `void`.

### Internal Helpers

#### `void draw_futuristic_meter(int y, int x, const char* label, int percentage, int base_pair)`

*   **Description**: Draws a sleek futuristic progress bar (`━━━╸`) with dynamic color highlighting based on utilization.

#### `void draw_pill_footer(int* x, int max_y, const char* key, const char* desc)`

*   **Description**: Draws high-tech bracketed footer items (`⟨KEY⟩ DESC`) used for the shortcut navigation bar.
