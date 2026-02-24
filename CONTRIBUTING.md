# Contributing to ProcX

We welcome contributions to ProcX! This document outlines how you can contribute to the project, whether it's by reporting bugs, suggesting features, or submitting code changes.

## Code of Conduct

Please note that this project is released with a [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project, you agree to abide by its terms.

## How Can I Contribute?

### Reporting Bugs

If you find a bug, please help us by reporting it!
*   **Check existing issues**: Before creating a new issue, please check if the bug has already been reported.
*   **Use the Bug Report template**: When reporting a bug, please use our [Bug Report template](.github/ISSUE_TEMPLATE/bug_report.md). This ensures we get all the necessary information to reproduce and fix the issue.

### Suggesting Enhancements

Have an idea for a new feature or an improvement to an existing one? We'd love to hear it!
*   **Check existing issues**: Before creating a new feature request, please check if the idea has already been suggested.
*   **Use the Feature Request template**: When suggesting an enhancement, please use our [Feature Request template](.github/ISSUE_TEMPLATE/feature_request.md).

### Asking Questions

If you have a question about how to use ProcX or its functionalities, feel free to ask!
*   **Use the Question template**: For general questions, please use our [Question template](.github/ISSUE_TEMPLATE/question.md).

### Submitting Pull Requests

We gladly accept pull requests for bug fixes, new features, and improvements.

1.  **Fork the repository**: Start by forking the ProcX repository to your GitHub account.
2.  **Clone your fork**: Clone your forked repository to your local machine.
    ```bash
    git clone https://github.com/f0xptr/procx.git
    cd procx
    ```
3.  **Create a new branch**: Create a new branch for your changes.
    ```bash
    git checkout -b feature/your-feature-name-or-bugfix/issue-number
    ```
    (e.g., `git checkout -b feature/add-new-sorting-option` or `git checkout -b bugfix/fix-memory-leak-in-process-list/123`)
4.  **Make your changes**: Implement your bug fix or feature.
    *   Ensure your code adheres to the existing coding style (refer to `.clang-format` and `.editorconfig`).
    *   Add or update Doxygen comments for any new or modified functions/structures.
    *   Write or update tests to cover your changes.
5.  **Test your changes**: Make sure all existing tests pass and your new tests also pass.
    ```bash
    make test
    ```
6.  **Commit your changes**: Write clear, concise commit messages.
    ```bash
    git commit -m "feat: Add new sorting option to process list"
    ```
    (Please follow [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) if possible, e.g., `fix:`, `feat:`, `docs:`, `chore:`, `refactor:`)
7.  **Push to your fork**: Push your changes to your fork on GitHub.
    ```bash
    git push origin feature/your-feature-name-or-bugfix/issue-number
    ```
8.  **Create a Pull Request**: Go to the original ProcX repository on GitHub and create a new Pull Request from your branch.
    *   **Use the Pull Request template**: When creating your PR, please use our [Pull Request template](.github/PULL_REQUEST_TEMPLATE.md).
    *   Ensure your PR description is detailed, explaining the changes, their motivation, and how they were tested.

## Development Environment Setup

Refer to the [README.md](README.md) for instructions on setting up your development environment, including prerequisites and building the project.

Thank you for contributing to ProcX!
