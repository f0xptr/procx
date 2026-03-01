/**
 * @file main.c
 * @brief Entry point for ProcX with interactive loop and advanced features.
 * @version 2.0.0
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "../include/ui/display.h"
#include "../include/system/process_list.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/resource.h>

/**
 * @brief Main function of the ProcX application.
 */
int main() {
    init_ui();
    nodelay(stdscr, TRUE);

    int  ch;
    int  scroll_offset                          = 0;
    int  selection_idx                          = 0;
    int  refresh_rate                           = 1000;  // ms
    char search_query[64]                       = "";
    char sort_col[10]                           = "CPU%";
    int (*sort_cmp)(ProcessNode*, ProcessNode*) = cmp_cpu;

    while (1) {
        timeout(refresh_rate);
        ProcessNode* process_list = build_process_list();

        // Apply sorting
        if (sort_cmp) {
            sort_process_list(&process_list, sort_cmp);
        }

        render_dashboard(process_list, scroll_offset, selection_idx, search_query, sort_col);

        ch = getch();
        if (ch == 'q' || ch == 'Q' || ch == KEY_F(10) || ch == 27) {
            free_process_list(process_list);
            break;
        } else if (ch == KEY_DOWN) {
            selection_idx++;
            // Calculate filtered count
            int          count = 0;
            ProcessNode* curr  = process_list;
            while (curr) {
                if (search_query[0] == '\0' || strcasestr(curr->name, search_query) != NULL) {
                    count++;
                }
                curr = curr->next;
            }
            if (selection_idx >= count) selection_idx = count - 1;
            if (selection_idx < 0) selection_idx = 0;

            int max_y = getmaxy(stdscr);
            if (selection_idx - scroll_offset >= max_y - 10) {
                scroll_offset++;
            }
        } else if (ch == KEY_UP) {
            selection_idx--;
            if (selection_idx < 0) selection_idx = 0;
            if (selection_idx < scroll_offset) {
                scroll_offset--;
            }
        } else if (ch == KEY_F(1)) {
            render_help();
        } else if (ch == KEY_F(3)) {
            sort_cmp = cmp_cpu;
            strcpy(sort_col, "CPU%");
        } else if (ch == KEY_F(4)) {
            sort_cmp = cmp_mem;
            strcpy(sort_col, "MEM");
        } else if (ch == KEY_F(5)) {
            sort_cmp = cmp_name;
            strcpy(sort_col, "NAME");
        } else if (ch == KEY_F(6)) {
            sort_cmp = cmp_pid;
            strcpy(sort_col, "PID");
        } else if (ch == KEY_F(7) || ch == KEY_F(8)) {
            // Decrease or Increase Nice Value
            ProcessNode* curr  = process_list;
            int          count = 0;
            while (curr) {
                if (search_query[0] == '\0' || strcasestr(curr->name, search_query) != NULL) {
                    if (count == selection_idx) {
                        int current_nice = getpriority(PRIO_PROCESS, curr->pid);
                        int new_nice     = (ch == KEY_F(7)) ? current_nice - 1 : current_nice + 1;
                        if (new_nice >= -20 && new_nice <= 19) {
                            setpriority(PRIO_PROCESS, curr->pid, new_nice);
                        }
                        break;
                    }
                    count++;
                }
                curr = curr->next;
            }
        } else if (ch == '\n' || ch == KEY_ENTER) {
            // New Feature: Show Process Details
            ProcessNode* curr  = process_list;
            int          count = 0;
            while (curr) {
                if (search_query[0] == '\0' || strcasestr(curr->name, search_query) != NULL) {
                    if (count == selection_idx) {
                        render_process_details(curr);
                        break;
                    }
                    count++;
                }
                curr = curr->next;
            }
        } else if (ch == '/') {
            // Integrated search input
            mvprintw(5, 2, "FILTER: ");
            clrtoeol();
            echo();
            curs_set(1);
            getnstr(search_query, sizeof(search_query) - 1);
            noecho();
            curs_set(0);
            selection_idx = 0;
            scroll_offset = 0;
        } else if (ch == 'h' || ch == 'H') {
            render_help();
        } else if (ch == 'k' || ch == 'K' || ch == KEY_F(9)) {
            // Kill selected process
            ProcessNode* curr  = process_list;
            int          count = 0;
            while (curr) {
                if (search_query[0] == '\0' || strcasestr(curr->name, search_query) != NULL) {
                    if (count == selection_idx) {
                        if (render_confirmation(curr->pid)) {
                            kill(curr->pid, SIGTERM);
                        }
                        break;
                    }
                    count++;
                }
                curr = curr->next;
            }
        }

        free_process_list(process_list);
    }

    close_ui();
    return 0;
}
