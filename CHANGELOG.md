# Changelog

All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.0] - 2026-03-02

### Added
*   **Futuristic Cyber-Dark UI**: Complete visual overhaul with a neon-inspired aesthetic, utilizing high-contrast colors (Cyan, Magenta, Yellow) and sleek Unicode symbols.
*   **Process Inspector**: New detailed view accessible via `ENTER` key, providing comprehensive process metadata (UID, PPID, exact memory, CPU ticks).
*   **Real-time Priority Management**: Added support for adjusting process priority (`NI`/Nice value) directly from the UI using `F7` (Decrease) and `F8` (Increase).
*   **Full Descriptive Status**: Replaced single-letter process states with clear, human-readable labels (RUNNING, SLEEPING, ZOMBIE, etc.) for better accessibility.
*   **PID Sorting**: Added `F6` shortcut to sort the process list by Process ID.
*   **Advanced Resource Meters**: Redesigned CPU, Memory, and Swap meters with a sleek `━━━╸` futuristic style and dynamic color-coding.
*   **Integrated System Metrics**: Repositioned system-wide stats (Tasks, Load, Uptime) for a cleaner, more integrated dashboard layout.
*   **Modern Bracketed Footer**: Refined shortcut navigation with high-tech bracketed keys and elegant separators.
*   **Wide Character Support**: Switched to `ncursesw` for full Unicode compatibility and better rendering of modern terminal symbols.

### Changed
*   **Layout Refinement**: Optimized column spacing and alignment for a "pixel-perfect" tech-grid look.
*   **Smart Truncation**: Improved command name handling to prevent layout breaking on smaller terminal windows.
*   **Build System**: Updated `Makefile` to link against `libncursesw`.

## [1.1.1] - 2026-03-01

### Removed
*   **Hardcoded Versioning**: Removed manual version strings from `README.md` and `display.c`.

## [1.1.0] - 2026-02-27

### Added

*   **Process Selection**: Interactive selection bar to highlight specific processes.
*   **Process Management**: Ability to kill processes directly from the UI using the 'K' key with a confirmation dialog.
*   **Per-Process CPU Usage**: Real-time CPU percentage calculation for each individual process.
*   **Dynamic Sorting**: Sort processes by PID (F1), CPU usage (F3), Memory (F4), or Name (F5).
*   **Real-time Filtering**: Search and filter processes by name using the '/' key.
*   **System Metrics**: Added System Uptime and Load Average (1, 5, 15 min) to the dashboard header.
*   **Help Overlay**: Interactive help screen accessible via 'H' key.
*   **Refresh Rate Control**: Adjust the dashboard update frequency using '+' and '-' keys.
*   **Improved Robustness**: Better handling of `/proc` parsing and permission restrictions.

### Changed

*   Updated UI layout to accommodate new metrics and information columns.
*   Refactored process information gathering for better accuracy and performance.
*   Enhanced Doxygen documentation throughout the codebase with professional English grammar.

## [1.0.0] - 2026-02-24

### Added

*   Initial release of ProcX.
*   Real-time process listing with PID, USER, PPID, THR, S, MEM(KB), COMMAND.
*   System resource meters for CPU, MEM, SWP usage.
*   Interactive UI with ncurses, including scrolling functionality.
*   Basic unit test for system information parsing.
*   Comprehensive Doxygen comments for C source and header files, ensuring consistent style and good English grammar.
*   Detailed Makefile for build and test automation, with extensive comments.
*   Professional README.md with build, run, test, and contribution instructions.
*   MIT License for the project.
*   Integrated GitHub Actions for Continuous Integration (CI).
    *   Basic build and test workflow (`.github/workflows/ci.yml`) with comments.
    *   Dependabot configuration for GitHub Actions dependencies (`.github/dependabot.yml`).
*   Added comprehensive contribution guidelines:
    *   `CONTRIBUTING.md` with guidelines for bug reports, feature requests, and pull requests.
    *   `PULL_REQUEST_TEMPLATE.md` for structured pull requests.
    *   Dedicated issue templates (`bug_report.md`, `feature_request.md`, `question.md`) in `.github/ISSUE_TEMPLATE/`.
    *   `config.yml` to streamline issue reporting.
    *   `CODE_OF_CONDUCT.md` based on Contributor Covenant.
*   Improved project documentation:
    *   Modular codebase explanation in `docs/` (`core/process.md`, `system/sys_info.md`, etc.).
    *   `CONTRIBUTORS.md` to list project contributors (initially, f0xptr).
*   Introduced development scripts:
    *   `scripts/format.sh` for code formatting with `clang-format`.
    *   `scripts/lint.sh` for static analysis with `clang-tidy`.
    *   `scripts/example.sh` as a placeholder.
*   Added editor and Git configurations:
    *   `.clang-format` for consistent code style.
    *   `.clang-format-ignore` to exclude specific files from formatting.
    *   `.clang-tidy.in` template for linting configuration.
    *   `.editorconfig` for cross-editor consistency.
    *   `.gitattributes` to enforce line endings and handle binary files.
*   This CHANGELOG.md file.
