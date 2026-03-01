/**
 * @file display.c
 * @brief Final Polished "Cyber-Dark" UI for ProcX.
 * @version 2.0.0
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define _XOPEN_SOURCE_EXTENDED 1
#include "../../include/ui/display.h"
#include "../../include/system/sys_info.h"
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// Neon Futuristic Color Palette
#define CP_DEFAULT 1
#define CP_CYAN 2     // Primary Neon
#define CP_MAGENTA 3  // Secondary Neon
#define CP_RED 4      // Danger
#define CP_YELLOW 5   // Warning
#define CP_GREEN 6    // Success
#define CP_HEADER 7
#define CP_SELECT 8
#define CP_FOOTER_KEY 9
#define CP_FOOTER_DESC 10
#define CP_DIM 11
#define CP_ACCENT 12

void init_ui() {
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    init_pair(CP_DEFAULT, -1, -1);
    init_pair(CP_CYAN, COLOR_CYAN, -1);
    init_pair(CP_MAGENTA, COLOR_MAGENTA, -1);
    init_pair(CP_RED, COLOR_RED, -1);
    init_pair(CP_YELLOW, COLOR_YELLOW, -1);
    init_pair(CP_GREEN, COLOR_GREEN, -1);
    init_pair(CP_HEADER, COLOR_BLACK, COLOR_CYAN);
    init_pair(CP_SELECT, COLOR_BLACK, COLOR_CYAN);
    init_pair(CP_FOOTER_KEY, COLOR_CYAN, COLOR_BLACK);
    init_pair(CP_FOOTER_DESC, COLOR_BLACK, COLOR_CYAN);
    init_pair(CP_DIM, COLOR_WHITE, -1);
    init_pair(CP_ACCENT, COLOR_MAGENTA, -1);
}

/**
 * @brief Sleek Futuristic Meter ━━━╸
 */
void draw_futuristic_meter(int y, int x, const char* label, int percentage, int base_pair) {
    attron(COLOR_PAIR(base_pair) | A_BOLD);
    mvprintw(y, x, "%-4s", label);
    attroff(COLOR_PAIR(base_pair) | A_BOLD);

    int width  = 20;
    int filled = (percentage * width) / 100;

    attron(A_DIM);
    addstr(" ▕");
    attroff(A_DIM);

    for (int i = 0; i < width; i++) {
        if (i < filled) {
            attron(COLOR_PAIR(base_pair) | A_BOLD);
            addstr("━");
            attroff(COLOR_PAIR(base_pair) | A_BOLD);
        } else if (i == filled) {
            attron(COLOR_PAIR(base_pair));
            addstr("╸");
            attroff(COLOR_PAIR(base_pair));
        } else {
            attron(A_DIM);
            addstr("─");
            attroff(A_DIM);
        }
    }

    attron(A_DIM);
    addstr("▏");
    attroff(A_DIM);

    attron(COLOR_PAIR(base_pair) | A_BOLD);
    printw(" %3d%%", percentage);
    attroff(COLOR_PAIR(base_pair) | A_BOLD);
}

/**
 * @brief Refined Cyber Footer Item
 */
void draw_pill_footer(int* x, int max_y, const char* key, const char* desc) {
    attron(COLOR_PAIR(CP_MAGENTA) | A_BOLD);
    mvprintw(max_y - 1, *x, "⟨");
    *x += 1;
    attroff(COLOR_PAIR(CP_MAGENTA) | A_BOLD);

    attron(COLOR_PAIR(CP_CYAN) | A_BOLD);
    mvprintw(max_y - 1, *x, "%s", key);
    *x += strlen(key);
    attroff(COLOR_PAIR(CP_CYAN) | A_BOLD);

    attron(COLOR_PAIR(CP_MAGENTA) | A_BOLD);
    mvprintw(max_y - 1, *x, "⟩");
    *x += 1;
    attroff(COLOR_PAIR(CP_MAGENTA) | A_BOLD);

    attron(COLOR_PAIR(CP_DEFAULT) | A_BOLD);
    mvprintw(max_y - 1, *x, " %-8s", desc);
    *x += 9;
    attroff(COLOR_PAIR(CP_DEFAULT) | A_BOLD);

    attron(A_DIM);
    mvprintw(max_y - 1, *x, "┊ ");
    *x += 2;
    attroff(A_DIM);
}

void render_dashboard(ProcessNode* head, int scroll_offset, int selection_idx,
                      const char* search_query, const char* sort_col) {
    erase();
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    SystemInfo sys_info;
    get_system_info(&sys_info, head);

    // Resources
    int stats_x = 42;
    draw_futuristic_meter(1, 2, "CPU", sys_info.cpu_usage, CP_CYAN);
    attron(COLOR_PAIR(CP_CYAN) | A_BOLD);
    mvprintw(1, stats_x, "◸ TASKS ");
    attroff(COLOR_PAIR(CP_CYAN) | A_BOLD);
    printw(": %d ", sys_info.total_tasks);
    attron(A_DIM);
    printw("(%dR)", sys_info.running_tasks);
    attroff(A_DIM);

    draw_futuristic_meter(2, 2, "MEM", sys_info.mem_usage, CP_MAGENTA);
    attron(COLOR_PAIR(CP_MAGENTA) | A_BOLD);
    mvprintw(2, stats_x, "◸ LOAD  ");
    attroff(COLOR_PAIR(CP_MAGENTA) | A_BOLD);
    printw(": %.2f %.2f %.2f", sys_info.load_avg[0], sys_info.load_avg[1], sys_info.load_avg[2]);

    draw_futuristic_meter(3, 2, "SWP", sys_info.swp_usage, CP_YELLOW);
    attron(COLOR_PAIR(CP_YELLOW) | A_BOLD);
    mvprintw(3, stats_x, "◸ UPTIME");
    attroff(COLOR_PAIR(CP_YELLOW) | A_BOLD);
    int hh = sys_info.uptime_sec / 3600;
    int mm = (sys_info.uptime_sec % 3600) / 60;
    int ss = sys_info.uptime_sec % 60;
    printw(": %02d:%02d:%02d", hh, mm, ss);

    // Filter Info
    if (search_query[0] != '\0') {
        attron(A_BOLD | COLOR_PAIR(CP_MAGENTA));
        mvprintw(5, 2, " ❯ FILTER: ");
        attroff(A_BOLD | COLOR_PAIR(CP_MAGENTA));
        printw("%s", search_query);
    }

    // Precise Table Header
    int header_y = 6;
    attron(COLOR_PAIR(CP_HEADER) | A_BOLD);
    mvhline(header_y, 0, ' ', max_x);
    mvprintw(header_y, 1, "  %-7s  %-12s  %-4s  %-4s  %-8s  %-8s  %-10s  %-7s  %-s", "ID", "OWNER",
             "PRI", "NI", "VIRT", "RES", "STATUS", "CPU%", "COMMAND");

    // Exact Sort Highlighting
    if (strcmp(sort_col, "PID") == 0)
        mvprintw(header_y, 3, "ID");
    else if (strcmp(sort_col, "CPU%") == 0)
        mvprintw(header_y, 66, "CPU%%");
    else if (strcmp(sort_col, "MEM") == 0)
        mvprintw(header_y, 44, "RES");
    else if (strcmp(sort_col, "NAME") == 0)
        mvprintw(header_y, 76, "COMMAND");
    attroff(COLOR_PAIR(CP_HEADER) | A_BOLD);

    // Process Datastream
    ProcessNode* curr = head;
    int          row  = header_y + 1;
    int          idx  = 0;

    while (curr && row < max_y - 1) {
        if (search_query[0] != '\0' && strcasestr(curr->name, search_query) == NULL) {
            curr = curr->next;
            continue;
        }

        if (idx >= scroll_offset) {
            bool is_sel = (idx == selection_idx);
            if (is_sel) {
                attron(COLOR_PAIR(CP_SELECT) | A_BOLD);
                mvhline(row, 0, ' ', max_x);
            }

            // Column: ID
            if (!is_sel) attron(COLOR_PAIR(CP_CYAN) | A_BOLD);
            mvprintw(row, 1, "› %-6d", curr->pid);
            if (!is_sel) attroff(COLOR_PAIR(CP_CYAN) | A_BOLD);

            attron(A_DIM);
            mvaddstr(row, 9, "┆");
            attroff(A_DIM);

            // Column: Owner
            mvprintw(row, 11, "%-12.12s", curr->username);
            attron(A_DIM);
            mvaddstr(row, 24, "┆");
            attroff(A_DIM);

            // Column: PRI/NI/VIRT/RES
            mvprintw(row, 26, "%-4ld %-4ld %-8d %-8.1f", curr->priority, curr->nice_value,
                     (int)(curr->memory_kb * 1.1), (float)curr->memory_kb / 1024.0);
            attron(A_DIM);
            mvaddstr(row, 53, "┆");
            attroff(A_DIM);

            // Column: Status (Full Text)
            const char* status_text = "UNKNOWN";
            int         s_color     = CP_DEFAULT;
            switch (curr->state) {
                case 'R':
                    status_text = "RUNNING";
                    s_color     = CP_GREEN;
                    break;
                case 'S':
                    status_text = "SLEEPING";
                    s_color     = CP_CYAN;
                    break;
                case 'D':
                    status_text = "WAITING";
                    s_color     = CP_YELLOW;
                    break;
                case 'Z':
                    status_text = "ZOMBIE";
                    s_color     = CP_RED;
                    break;
                case 'T':
                    status_text = "STOPPED";
                    s_color     = CP_MAGENTA;
                    break;
                case 'I':
                    status_text = "IDLE";
                    s_color     = CP_DIM;
                    break;
            }

            if (!is_sel) attron(COLOR_PAIR(s_color) | A_BOLD);
            mvprintw(row, 55, "%-10s", status_text);
            if (!is_sel) attroff(COLOR_PAIR(s_color) | A_BOLD);

            attron(A_DIM);
            mvaddstr(row, 65, "┆");
            attroff(A_DIM);

            // Column: CPU%
            if (!is_sel) attron(COLOR_PAIR(CP_YELLOW) | A_BOLD);
            mvprintw(row, 67, "%-6.1f%%", curr->cpu_usage);
            if (!is_sel) attroff(COLOR_PAIR(CP_YELLOW) | A_BOLD);

            attron(A_DIM);
            mvaddstr(row, 74, "┆");
            attroff(A_DIM);

            // Column: Command
            mvprintw(row, 76, "%.*s", max_x - 77, curr->name);

            if (is_sel) attroff(COLOR_PAIR(CP_SELECT) | A_BOLD);
            row++;
        }
        curr = curr->next;
        idx++;
    }

    // Command Footer
    int fx = 1;
    mvhline(max_y - 1, 0, ' ', max_x);
    draw_pill_footer(&fx, max_y, "F1", "HELP");
    draw_pill_footer(&fx, max_y, "F3", "CPU%");
    draw_pill_footer(&fx, max_y, "F4", "MEM");
    draw_pill_footer(&fx, max_y, "F6", "PID");
    draw_pill_footer(&fx, max_y, "F9", "KILL");
    draw_pill_footer(&fx, max_y, "ENT", "INFO");
    draw_pill_footer(&fx, max_y, "ESC", "QUIT");

    refresh();
}

void render_process_details(ProcessNode* proc) {
    if (!proc) return;
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    int w = 66, h = 18;
    int x = (max_x - w) / 2, y = (max_y - h) / 2;

    WINDOW* win = newwin(h, w, y, x);
    keypad(win, TRUE);
    wbkgd(win, COLOR_PAIR(CP_DEFAULT));
    wattron(win, COLOR_PAIR(CP_CYAN));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(CP_CYAN));

    wattron(win, COLOR_PAIR(CP_CYAN) | A_BOLD);
    mvwprintw(win, 0, (w - 24) / 2, " ❯❯ PROCESS_INSPECTOR ");
    wattroff(win, COLOR_PAIR(CP_CYAN) | A_BOLD);

    mvwprintw(win, 2, 4, "┌─ IDENTIFICATION ─────────────────────────────┐");
    mvwprintw(win, 3, 4, "│ NAME : %-37s │", proc->name);
    mvwprintw(win, 4, 4, "│ PID  : %-10d  PPID : %-10d      │", proc->pid, proc->ppid);
    mvwprintw(win, 5, 4, "│ USER : %-10s  UID  : %-10d      │", proc->username, proc->uid);
    mvwprintw(win, 6, 4, "└──────────────────────────────────────────────┘");

    mvwprintw(win, 8, 4, "┌─ PERFORMANCE ────────────────────────────────┐");
    mvwprintw(win, 9, 4, "│ CPU  : %-7.2f%%    THREADS : %-10d    │", proc->cpu_usage,
              proc->num_threads);
    mvwprintw(win, 10, 4, "│ MEM  : %-7.2f MB   STATE   : %-10c    │",
              (float)proc->memory_kb / 1024.0, proc->state);
    mvwprintw(win, 11, 4, "└──────────────────────────────────────────────┘");

    wattron(win, A_DIM | COLOR_PAIR(CP_CYAN));
    mvwprintw(win, h - 2, (w - 28) / 2, "PRESS ANY KEY TO DISMISS");
    wattroff(win, A_DIM | COLOR_PAIR(CP_CYAN));

    wrefresh(win);
    wgetch(win);
    delwin(win);
}

void render_help() {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    int w = 54, h = 14;
    int x = (max_x - w) / 2, y = (max_y - h) / 2;

    WINDOW* win = newwin(h, w, y, x);
    wbkgd(win, COLOR_PAIR(CP_DEFAULT));
    wattron(win, COLOR_PAIR(CP_MAGENTA));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(CP_MAGENTA));

    wattron(win, COLOR_PAIR(CP_HEADER) | A_BOLD);
    mvwprintw(win, 0, (w - 18) / 2, " SYSTEM_COMMANDS ");
    wattroff(win, COLOR_PAIR(CP_HEADER) | A_BOLD);

    mvwprintw(win, 2, 4, "▲/▼      : Navigate Datastreams");
    mvwprintw(win, 4, 4, "F3..F6   : Reorder by Metrics");
    mvwprintw(win, 5, 4, "F7/F8    : Adjust Priority (NI)");
    mvwprintw(win, 6, 4, "F9 / K   : Terminate Task");
    mvwprintw(win, 7, 4, "/        : Dynamic Filter");
    mvwprintw(win, 8, 4, "ENTER    : Inspect Process");
    mvwprintw(win, 9, 4, "ESC / Q  : Shutdown ProcX");

    wattron(win, A_BOLD | COLOR_PAIR(CP_CYAN));
    mvwprintw(win, h - 2, (w - 22) / 2, "READY TO CONTINUE");
    wattroff(win, A_BOLD | COLOR_PAIR(CP_CYAN));

    wrefresh(win);
    wgetch(win);
    delwin(win);
}

int render_confirmation(int pid) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    int w = 44, h = 7;
    int x = (max_x - w) / 2, y = (max_y - h) / 2;

    WINDOW* win = newwin(h, w, y, x);
    wbkgd(win, COLOR_PAIR(CP_DEFAULT));
    wattron(win, COLOR_PAIR(CP_RED));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(CP_RED));

    wattron(win, A_BOLD | COLOR_PAIR(CP_RED));
    mvwprintw(win, 2, (w - 24) / 2, "!! DANGER: KILL PID %d !!", pid);
    wattroff(win, A_BOLD | COLOR_PAIR(CP_RED));
    mvwprintw(win, 4, (w - 24) / 2, "[Y] EXECUTE   [N] ABORT");

    wrefresh(win);
    int ch = wgetch(win);
    delwin(win);
    return (ch == 'y' || ch == 'Y');
}

void close_ui() { endwin(); }
