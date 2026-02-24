/**
 * @file display.c
 * @brief Implementation of the modern, aesthetic ncurses UI.
 * @version 1.0.0
 */

#include "../../include/ui/display.h"
#include "../../include/system/sys_info.h"
#include <ncurses.h>
#include <string.h>

void init_ui() {
    initscr();
    start_color();
    use_default_colors();  // Allows terminal transparency (-1 for default bg)
    cbreak();
    noecho();
    curs_set(0);           // Hide the cursor
    keypad(stdscr, TRUE);  // Enable arrow keys

    // Modern Color Palette
    init_pair(1, COLOR_GREEN, -1);          // Running / Good
    init_pair(2, COLOR_CYAN, -1);           // Sleeping / Normal
    init_pair(3, COLOR_WHITE, -1);          // Standard Text
    init_pair(4, COLOR_YELLOW, -1);         // Memory / Warnings
    init_pair(5, COLOR_RED, -1);            // Zombie / Critical
    init_pair(6, COLOR_BLACK, COLOR_CYAN);  // Table Header (Cyan BG, Black Text)
    init_pair(7, COLOR_WHITE, COLOR_BLUE);  // Title Bar (Blue BG, White Text)
    init_pair(8, COLOR_MAGENTA, -1);        // Highlights
    init_pair(9, COLOR_BLUE, -1);           // Borders
}

/**
 * @brief Helper function to draw an htop-style meter.
 *
 * This function renders a progress bar with a label, percentage, and
 * optional usage/total values. It's designed to visually represent resource
 * usage like CPU, memory, or swap.
 *
 * @param y The Y-coordinate (row) where the meter should be drawn.
 * @param x The X-coordinate (column) where the meter should be drawn.
 * @param label The text label displayed before the meter.
 * @param percentage The percentage value (0-100) to represent on the meter.
 * @param color_pair The ncurses color pair to use for the meter's filled portion.
 * @param used The amount of resource currently used (e.g., KB, MB).
 * @param total The total available amount of the resource.
 * @param unit The unit of measurement for used and total (e.g., "MB", "KB").
 */
void draw_meter(int y, int x, const char* label, int percentage, int color_pair, long used,
                long total, const char* unit) {
    mvprintw(y, x, "%-4s [", label);
    attron(COLOR_PAIR(color_pair) | A_BOLD);

    int bars = percentage / 2;  // 50 characters wide max
    for (int i = 0; i < 50; i++) {
        if (i < bars)
            printw("|");
        else
            printw(" ");
    }

    attroff(COLOR_PAIR(color_pair) | A_BOLD);
    printw("] %5.1f%%", (float)percentage);

    if (total > 0) {
        attron(COLOR_PAIR(3));
        printw(" %5ld/%ld %s", used, total, unit);
        attroff(COLOR_PAIR(3));
    }
}

void render_dashboard(ProcessNode* head, int scroll_offset) {
    clear();
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);  // Get terminal window size

    SystemInfo sys_info;
    get_system_info(&sys_info, head);

    // Top title bar
    attron(COLOR_PAIR(7) | A_BOLD);
    mvhline(0, 0, ' ', max_x);  // Fill full width with background color
    mvprintw(0, 2, " ProcX - Advanced Process Explorer ");
    mvprintw(0, max_x - 30, "Tasks: %d, %d thr; %d running", sys_info.total_tasks,
             sys_info.total_tasks * 2, sys_info.running_tasks);  // approx threads if we don't sum
    attroff(COLOR_PAIR(7) | A_BOLD);

    // System meters (htop style)
    long mem_used = sys_info.total_mem_kb - sys_info.free_mem_kb;
    long swp_used = sys_info.total_swp_kb - sys_info.free_swp_kb;

    draw_meter(2, 2, "CPU", sys_info.cpu_usage, 1, 0, 0, "");
    draw_meter(3, 2, "MEM", sys_info.mem_usage, 4, mem_used / 1024, sys_info.total_mem_kb / 1024,
               "MB");
    draw_meter(4, 2, "SWP", sys_info.swp_usage, 5, swp_used / 1024, sys_info.total_swp_kb / 1024,
               "MB");

    // Borders and separators
    attron(COLOR_PAIR(9));
    mvhline(6, 0, ACS_HLINE, max_x);  // Separator line
    attroff(COLOR_PAIR(9));

    // Table header
    int header_y = 7;
    attron(COLOR_PAIR(6) | A_BOLD);
    mvhline(header_y, 0, ' ', max_x);  // Fill full width
    // Updated columns: PID, USER, PPID, THR, S, MEM (KB), COMMAND
    mvprintw(header_y, 2, "%-8s %-12s %-8s %-5s %-4s %-12s %-25s", "PID", "USER", "PPID", "THR",
             "S", "MEM(KB)", "COMMAND");
    attroff(COLOR_PAIR(6) | A_BOLD);

    // Dynamic process list
    ProcessNode* current     = head;
    int          row         = header_y + 1;
    int          current_idx = 0;

    // Skip nodes based on scrolling
    while (current != NULL && current_idx < scroll_offset) {
        current = current->next;
        current_idx++;
    }

    // Render visible rows
    while (current != NULL && row < max_y - 2) {  // Leave bottom 2 rows for footer and spacing
        // Dynamic color logic based on state
        int state_color = 3;  // Default White
        if (current->state == 'R')
            state_color = 1;  // Green
        else if (current->state == 'S')
            state_color = 2;  // Cyan
        else if (current->state == 'Z')
            state_color = 5;  // Red (Zombie)

        // Print PID
        attron(COLOR_PAIR(3));
        mvprintw(row, 2, "%-8d ", current->pid);
        attroff(COLOR_PAIR(3));

        // Print Username
        attron(COLOR_PAIR(8));
        printw("%-12.12s ", current->username);
        attroff(COLOR_PAIR(8));

        // Print PPID & Threads
        attron(COLOR_PAIR(3));
        printw("%-8d %-5d ", current->ppid, current->num_threads);
        attroff(COLOR_PAIR(3));

        // Print State (Color Coded)
        attron(COLOR_PAIR(state_color) | A_BOLD);
        printw("%-4c ", current->state);
        attroff(COLOR_PAIR(state_color) | A_BOLD);

        // Print Memory
        attron(COLOR_PAIR(4));
        printw("%-12ld ", current->memory_kb);
        attroff(COLOR_PAIR(4));

        // Print Command
        attron(COLOR_PAIR(3));
        printw("%-25.50s", current->name);  // allow longer names to flow
        attroff(COLOR_PAIR(3));

        current = current->next;
        row++;
    }

    // Footer bar
    attron(COLOR_PAIR(7));
    mvhline(max_y - 1, 0, ' ', max_x);
    mvprintw(max_y - 1, 2, " F10/Q Quit | UP/DOWN Scroll ");
    attroff(COLOR_PAIR(7));

    refresh();
}

void close_ui() { endwin(); }
