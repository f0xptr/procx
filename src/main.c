/**
 * @file main.c
 * @brief Entry point for ProcX with interactive loop and advanced features.
 * @version 1.1.0
 */

#include "../include/ui/display.h"
#include "../include/system/process_list.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/**
 * @brief Main function of the ProcX application.
 */
int main() {
    init_ui();
    nodelay(stdscr, TRUE);
    
    int          ch;
    int          scroll_offset = 0;
    int          selection_idx = 0;
    int          refresh_rate  = 1000; // ms
    char         search_query[64] = "";
    char         sort_col[10] = "CPU%";
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
        if (ch == 'q' || ch == 'Q') {
            free_process_list(process_list);
            break;
        } else if (ch == KEY_DOWN) {
            selection_idx++;
            // Basic boundary check (could be improved by counting filtered list)
            int count = 0;
            ProcessNode* curr = process_list;
            while(curr) { count++; curr = curr->next; }
            if (selection_idx >= count) selection_idx = count - 1;
            
            // Adjust scroll if selection goes off screen
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
            sort_cmp = cmp_pid;
            strcpy(sort_col, "PID");
        } else if (ch == KEY_F(3)) {
            sort_cmp = cmp_cpu;
            strcpy(sort_col, "CPU%");
        } else if (ch == KEY_F(4)) {
            sort_cmp = cmp_mem;
            strcpy(sort_col, "MEM");
        } else if (ch == KEY_F(5)) {
            sort_cmp = cmp_name;
            strcpy(sort_col, "NAME");
        } else if (ch == '/') {
            // Simple search input
            echo();
            curs_set(1);
            mvprintw(5, 2, "Search: ");
            getnstr(search_query, sizeof(search_query) - 1);
            noecho();
            curs_set(0);
            selection_idx = 0;
            scroll_offset = 0;
        } else if (ch == 'h' || ch == 'H') {
            render_help();
        } else if (ch == '+' || ch == '=') {
            refresh_rate -= 100;
            if (refresh_rate < 100) refresh_rate = 100;
        } else if (ch == '-') {
            refresh_rate += 100;
            if (refresh_rate > 5000) refresh_rate = 5000;
        } else if (ch == 'k' || ch == 'K') {
            // Kill selected process
            ProcessNode* curr = process_list;
            for(int i=0; i<selection_idx && curr; i++) curr = curr->next;
            if (curr) {
                if (render_confirmation(curr->pid)) {
                    kill(curr->pid, SIGTERM);
                }
            }
        }

        free_process_list(process_list);
    }

    close_ui();
    return 0;
}
