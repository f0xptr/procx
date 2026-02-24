# Main Application Flow

This document outlines the main entry point and overall operational flow of the ProcX application, as defined in `main.c`.

## `main()` Function

The `main()` function serves as the heart of the ProcX application, orchestrating the initialization of the UI, the continuous data fetching and rendering cycle, and user interaction handling.

### Overview of Operations

1.  **UI Initialization**:
    *   Calls `init_ui()` to set up the ncurses environment, including color schemes and input handling.
    *   Configures `nodelay` and `timeout` for `stdscr` to enable non-blocking input and periodic screen refreshes.

2.  **Main Loop**:
    *   Enters an infinite loop that continues until the user decides to quit.
    *   **Process List Refresh**: In each iteration, it calls `build_process_list()` to dynamically scan `/proc` and create an up-to-date linked list of running processes.
    *   **Dashboard Rendering**: Calls `render_dashboard()` to draw the current system information and the process list on the terminal screen. The `scroll_offset` is passed to manage vertical scrolling.
    *   **Input Handling**: Checks for user input using `getch()`.
        *   If 'q' or 'Q' is pressed, the loop breaks, and the application exits.
        *   If `KEY_UP` or `KEY_DOWN` is pressed, the `scroll_offset` is adjusted to enable navigation through the process list.
    *   **Memory Management**: Calls `free_process_list()` at the end of each iteration to release the memory allocated for the process list, preventing memory leaks and ensuring a clean state for the next refresh.

3.  **UI Teardown**:
    *   After the main loop terminates, `close_ui()` is called to restore the terminal to its original state.

4.  **Exit**:
    *   The program exits with a return code of `0`, indicating successful execution.

## Data Flow

The `main` function acts as a central coordinator:

*   It orchestrates calls to `build_process_list()` from the `system` module to fetch raw process data.
*   It passes this data (the `ProcessNode` head pointer) to `render_dashboard()` from the `ui` module for visual presentation.
*   It manages user input to control the `ui` (scrolling) and the application's lifecycle (quitting).

This design ensures a clear separation of concerns, with `main` focusing on application flow rather than data acquisition or rendering logic.
