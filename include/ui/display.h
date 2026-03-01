/**
 * @file display.h
 * @brief UI functions using ncurses for an aesthetic terminal display.
 * @version 2.0.0
 */

#ifndef PROCX_DISPLAY_H
#define PROCX_DISPLAY_H

#include "../core/process.h"

/**
 * @brief Initializes the ncurses user interface.
 */
void init_ui();

/**
 * @brief Renders the main aesthetic dashboard.
 * @param head Pointer to the process list.
 * @param scroll_offset Number of rows to skip.
 * @param selection_idx Index of the currently selected process.
 * @param search_query Current search string.
 * @param sort_col Current sorting column name.
 */
void render_dashboard(ProcessNode* head, int scroll_offset, int selection_idx,
                      const char* search_query, const char* sort_col);

/**
 * @brief Renders the help overlay.
 */
void render_help();

/**
 * @brief Renders a confirmation dialog for killing a process.
 * @param pid The PID to kill.
 * @return 1 if confirmed, 0 otherwise.
 */
int render_confirmation(int pid);

/**
 * @brief Renders a detailed process view.
 * @param proc Pointer to the process to display.
 */
void render_process_details(ProcessNode* proc);

/**
 * @brief Cleans up and closes the ncurses interface.
 */
void close_ui();

#endif  // PROCX_DISPLAY_H
