# ProcX

ProcX is a lightweight, high-performance, ncurses-based process explorer and system monitoring tool for Linux. Written in C, it provides a modern, aesthetic, and real-time overview of your system's health, offering an interactive 'htop'-like experience directly in your terminal.

## Features

*   **Futuristic UI**: A complete "Cyber-Dark" visual overhaul with neon aesthetics, sleek Unicode meters (`━━━╸`), and elegant layout.
*   **Real-time Monitoring**: Live updates of CPU, Memory, and Swap utilization with dynamic color-coding.
*   **Process Inspector**: Inspect deep process metadata (UID, PPID, exact memory, CPU ticks) via a dedicated popup window (`ENTER`).
*   **Priority Management**: Adjust process priority (`NI`/Nice value) directly from the interface using `F7`/`F8`.
*   **Intelligent Filtering**: Dynamic search and filtering by process name using the `/` key.
*   **Advanced Table**: Professional columns including PID, Owner, Priority, Nice value, Virtual/Resident Memory, and full descriptive status.
*   **Dynamic Sorting**: Instantly reorder the process list by CPU, Memory, Name, or PID.
*   **Safety First**: Securely terminate (`SIGTERM`) processes with a dedicated confirmation prompt (`F9` or `K`).

## Building ProcX

To build ProcX from source, clone the repository and ensure you have the necessary dependencies.

```bash
git clone https://github.com/f0xptr/procx.git
cd procx
```

### Prerequisites

*   A C compiler (GCC or Clang)
*   `make` build tool
*   `ncurses` development libraries (with wide-character support)

**Debian/Ubuntu**:
```bash
sudo apt update && sudo apt install build-essential libncursesw5-dev
```

**Fedora/RHEL**:
```bash
sudo dnf install gcc make ncurses-devel
```

### Compilation

Run `make` in the root directory:
```bash
make
```
The `procx` executable will be generated in the project root.

## Running ProcX

```bash
./procx
```

### Keyboard Controls

| Key | Action |
|-----|--------|
| `UP` / `DOWN` | Navigate and select processes in the list |
| `F1` | Show **Help** menu |
| `F3` | Sort by **CPU%** |
| `F4` | Sort by **Memory usage** |
| `F5` | Sort by **Process Name** |
| `F6` | Sort by **PID** |
| `F7` | **Decrease Nice** value (Raise priority) |
| `F8` | **Increase Nice** value (Lower priority) |
| `F9` / `K` | **Kill** the selected process (requires confirmation) |
| `ENTER` | Open **Process Inspector** for details |
| `/` | **Search** / Filter processes by name |
| `ESC` / `Q` / `F10` | **Quit** ProcX |

## Running Tests

ProcX includes unit tests for system information parsing:
```bash
make test
```

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for our workflow and [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for community guidelines.

## License

ProcX is open-source software licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
