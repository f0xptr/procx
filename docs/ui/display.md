# UI: Ncurses Display Functions

This module handles all user interface rendering and interaction using the ncurses library, providing an aesthetic terminal-based dashboard for ProcX.

### Functions

### `void init_ui()`

*   **Description**: Initializes the ncurses environment. This includes setting up the screen, enabling color support, configuring input modes (cbreak, noecho), hiding the cursor, enabling special keys (like arrow keys), and defining the custom color pairs used throughout the application.
*   **Parameters**: None.
*   **Returns**: `void`.

### `void render_dashboard(ProcessNode *head, int scroll_offset)`

*   **Description**: Clears the screen and renders the entire ProcX dashboard. This involves displaying the title bar, system resource meters (CPU, Memory, Swap), the process table header, and a dynamic list of processes. The process list is rendered starting from an offset to support scrolling.
*   **Parameters**:
    *   `head`: A pointer to the head of the `ProcessNode` linked list, representing all active processes to be displayed.
    *   `scroll_offset`: An integer indicating the number of processes to skip from the beginning of the list, used for vertical scrolling.
*   **Returns**: `void`.

### `void close_ui()`

*   **Description**: Cleans up the ncurses environment, restoring the terminal to its original state.
*   **Parameters**: None.
*   **Returns**: `void`.

### Helper Function: `void draw_meter(...)`

*   **Description**: A static helper function used internally by `render_dashboard` to draw htop-style resource meters (e.g., for CPU, MEM, SWP). It renders a progress bar with a label, percentage, and usage/total values.
*   **Parameters**: (See `src/ui/display.c` for full parameter details).
    *   `y`, `x`: Screen coordinates for drawing.
    *   `label`: Text label for the meter.
    *   `percentage`: Current utilization percentage.
    *   `color_pair`: Ncurses color pair for the meter bar.
    *   `used`, `total`, `unit`: Optional usage details (e.g., MB, KB).
*   **Returns**: `void`.
