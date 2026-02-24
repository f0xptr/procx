# Changelog

All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
