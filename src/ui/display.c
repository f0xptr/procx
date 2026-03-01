/**
 * @file display.c
 * @brief Implementation of the modern, aesthetic ncurses UI.
 * @version 1.1.1
 */

#include "../../include/ui/display.h"
#include "../../include/system/sys_info.h"
#include <ncurses.h>
#include <string.h>
#include <time.h>

void init_ui() {
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Modern Color Palette
    init_pair(1, COLOR_GREEN, -1);            // Running / Good
    init_pair(2, COLOR_CYAN, -1);             // Sleeping / Normal
    init_pair(3, COLOR_WHITE, -1);            // Standard Text
    init_pair(4, COLOR_YELLOW, -1);           // Memory / Warnings
    init_pair(5, COLOR_RED, -1);              // Zombie / Critical
    init_pair(6, COLOR_BLACK, COLOR_CYAN);    // Table Header
    init_pair(7, COLOR_WHITE, COLOR_BLUE);    // Title Bar
    init_pair(8, COLOR_MAGENTA, -1);          // Highlights
    init_pair(9, COLOR_BLUE, -1);             // Borders
    init_pair(10, COLOR_BLACK, COLOR_WHITE);  // Selection Bar
}

void draw_meter(int y, int x, const char* label, int percentage, int color_pair, long used,
                long total, const char* unit) {
    mvprintw(y, x, "%-4s [", label);
    attron(COLOR_PAIR(color_pair) | A_BOLD);

    int bars = percentage / 2;
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

void render_dashboard(ProcessNode* head, int scroll_offset, int selection_idx,
                      const char* search_query, const char* sort_col) {
    clear();
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    SystemInfo sys_info;
    get_system_info(&sys_info, head);

    // Top title bar
    attron(COLOR_PAIR(7) | A_BOLD);
    mvhline(0, 0, ' ', max_x);
    mvprintw(0, 2, " ProcX ");

    // Uptime and Load Avg
    int hh = sys_info.uptime_sec / 3600;
    int mm = (sys_info.uptime_sec % 3600) / 60;
    int ss = sys_info.uptime_sec % 60;
    mvprintw(0, 20, "Uptime: %02d:%02d:%02d | Load: %.2f %.2f %.2f", hh, mm, ss,
             sys_info.load_avg[0], sys_info.load_avg[1], sys_info.load_avg[2]);

    mvprintw(0, max_x - 35, "Tasks: %d, %d running", sys_info.total_tasks, sys_info.running_tasks);
    attroff(COLOR_PAIR(7) | A_BOLD);

    // System meters
    long mem_used = sys_info.total_mem_kb - sys_info.free_mem_kb;
    long swp_used = sys_info.total_swp_kb - sys_info.free_swp_kb;

    draw_meter(2, 2, "CPU", sys_info.cpu_usage, 1, 0, 0, "");
    draw_meter(3, 2, "MEM", sys_info.mem_usage, 4, mem_used / 1024, sys_info.total_mem_kb / 1024,
               "MB");
    draw_meter(4, 2, "SWP", sys_info.swp_usage, 5, swp_used / 1024, sys_info.total_swp_kb / 1024,
               "MB");

    // Search query display
    if (search_query[0] != '\0') {
        attron(COLOR_PAIR(8) | A_BOLD);
        mvprintw(5, 2, "Search: %s", search_query);
        attroff(COLOR_PAIR(8) | A_BOLD);
    }

    attron(COLOR_PAIR(9));
    mvhline(6, 0, ACS_HLINE, max_x);
    attroff(COLOR_PAIR(9));

    // Table header
    int header_y = 7;
    attron(COLOR_PAIR(6) | A_BOLD);
    mvhline(header_y, 0, ' ', max_x);
    mvprintw(header_y, 2, "%-8s %-12s %-6s %-5s %-4s %-10s %-8s %-25s", "PID", "USER", "CPU%",
             "THR", "S", "MEM(MB)", "PPID", "COMMAND");

    // Highlight sorting column
    if (strcmp(sort_col, "PID") == 0)
        mvprintw(header_y, 2, "PID");
    else if (strcmp(sort_col, "CPU%") == 0)
        mvprintw(header_y, 11, "CPU%%");
    else if (strcmp(sort_col, "MEM") == 0)
        mvprintw(header_y, 35, "MEM(MB)");
    else if (strcmp(sort_col, "NAME") == 0)
        mvprintw(header_y, 55, "COMMAND");

    attroff(COLOR_PAIR(6) | A_BOLD);

    // Filter and Render visible rows
    ProcessNode* current        = head;
    int          row            = header_y + 1;
    int          current_idx    = 0;
    int          rendered_count = 0;

    while (current != NULL) {
        // Simple search filtering
        if (search_query[0] != '\0' && strcasestr(current->name, search_query) == NULL) {
            current = current->next;
            continue;
        }

        if (current_idx >= scroll_offset && row < max_y - 2) {
            if (current_idx == selection_idx) {
                attron(COLOR_PAIR(10));
                mvhline(row, 0, ' ', max_x);
            }

            int state_color = 3;
            if (current->state == 'R')
                state_color = 1;
            else if (current->state == 'S')
                state_color = 2;
            else if (current->state == 'Z')
                state_color = 5;

            mvprintw(row, 2, "%-8d %-12.12s %-6.1f %-5d %c    %-10.1f %-8d %-25.50s", current->pid,
                     current->username, current->cpu_usage, current->num_threads, current->state,
                     (float)current->memory_kb / 1024.0, current->ppid, current->name);

            if (current_idx == selection_idx) {
                attroff(COLOR_PAIR(10));
            } else {
                // Re-apply colors for state and memory if not selected
                attron(COLOR_PAIR(state_color) | A_BOLD);
                mvprintw(row, 31, "%c", current->state);
                attroff(COLOR_PAIR(state_color) | A_BOLD);

                attron(COLOR_PAIR(4));
                mvprintw(row, 36, "%-10.1f", (float)current->memory_kb / 1024.0);
                attroff(COLOR_PAIR(4));
            }
            row++;
            rendered_count++;
        }
        current = current->next;
        current_idx++;
    }

    // Footer bar
    attron(COLOR_PAIR(7));
    mvhline(max_y - 1, 0, ' ', max_x);
    mvprintw(max_y - 1, 2,
             " F1 PID | F3 CPU | F4 MEM | F5 NAME | / Search | K Kill | H Help | Q Quit ");
    attroff(COLOR_PAIR(7));

    refresh();
}

void render_help() {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    int w = 50, h = 15;
    int x = (max_x - w) / 2;
    int y = (max_y - h) / 2;

    WINDOW* help_win = newwin(h, w, y, x);
    box(help_win, 0, 0);
    mvwprintw(help_win, 1, (w - 10) / 2, " ProcX Help ");
    mvwprintw(help_win, 3, 2, "UP/DOWN   : Navigate process list");
    mvwprintw(help_win, 4, 2, "F1/F3/F4/F5: Sort by PID/CPU/MEM/NAME");
    mvwprintw(help_win, 5, 2, "/         : Search/Filter processes");
    mvwprintw(help_win, 6, 2, "K         : Kill selected process");
    mvwprintw(help_win, 7, 2, "+ / -     : Increase/Decrease refresh rate");
    mvwprintw(help_win, 8, 2, "Q         : Quit ProcX");
    mvwprintw(help_win, 10, 2, "Press any key to close...");
    wrefresh(help_win);
    wgetch(help_win);
    delwin(help_win);
}

int render_confirmation(int pid) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    int w = 40, h = 5;
    int x = (max_x - w) / 2;
    int y = (max_y - h) / 2;

    WINDOW* conf_win = newwin(h, w, y, x);
    box(conf_win, 0, 0);
    mvwprintw(conf_win, 1, 2, "Kill process %d?", pid);
    mvwprintw(conf_win, 3, 2, "Press 'Y' to confirm, any other to cancel");
    wrefresh(conf_win);

    int ch = wgetch(conf_win);
    delwin(conf_win);
    return (ch == 'y' || ch == 'Y');
}

void close_ui() { endwin(); }
