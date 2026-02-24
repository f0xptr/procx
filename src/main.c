/**
 * @file main.c
 * @brief Entry point for ProcX with interactive loop.
 * @version 1.0.0
 */

#include "../include/ui/display.h"
#include "../include/system/process_list.h"
#include <ncurses.h>

/**
 * @brief Main function of the ProcX application.
 * Initializes the user interface, continuously updates and renders the process dashboard,
 * and handles user input for navigation and exiting.
 * @return int Returns 0 on successful execution.
 */
int main() {
    init_ui();
    nodelay(stdscr, TRUE);  // Non-blocking getch
    timeout(1000);          // Wait max 1000ms for key press, then refresh screen

    int          ch;
    int          scroll_offset = 0;
    ProcessNode* process_list  = NULL;

    while (1) {
        // Build fresh list every tick (Lightweight enough for C)
        process_list = build_process_list();

        render_dashboard(process_list, scroll_offset);

        ch = getch();
        if (ch == 'q' || ch == 'Q') {
            free_process_list(process_list);
            break;
        } else if (ch == KEY_DOWN) {
            scroll_offset++;
        } else if (ch == KEY_UP && scroll_offset > 0) {
            scroll_offset--;
        }

        // Free memory to prevent memory leaks before next tick
        free_process_list(process_list);
    }

    close_ui();
    return 0;
}
