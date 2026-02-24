/**
 * @file display.h
 * @brief UI functions using ncurses for an aesthetic terminal display.
 * @version 1.0.0
 */

#ifndef PROCX_DISPLAY_H
#define PROCX_DISPLAY_H

#include "../core/process.h"

/**
 * @brief Initializes the ncurses user interface.
 * Sets up colors, hides the cursor, and enables keypad input.
 */
void init_ui();

/**
 * @brief Renders the main aesthetic dashboard with dynamic data.
 * * @param head Pointer to the head of the ProcessNode linked list.
 * @param scroll_offset The number of rows to skip (for scrolling functionality).
 */
void render_dashboard(ProcessNode* head, int scroll_offset);

/**
 * @brief Cleans up and closes the ncurses interface.
 * Restores the terminal to its normal state.
 */
void close_ui();

#endif  // PROCX_DISPLAY_H
