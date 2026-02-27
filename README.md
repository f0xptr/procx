# ProcX v1.1.0

ProcX is a lightweight, high-performance, ncurses-based process explorer and system monitoring tool for Linux. Written in C, it provides a modern, aesthetic, and real-time overview of your system's health, offering an interactive 'htop'-like experience directly in your terminal.

## Features

*   **Real-time Monitoring**: Live updates of CPU, Memory, and Swap utilization meters.
*   **Process Management**: Identify and terminate (`SIGTERM`) processes with a safety confirmation dialog.
*   **Detailed Insights**: Displays PID, User, CPU%, Threads, State, Memory (MB), PPID, and Command.
*   **Aesthetic UI**: Modern color-coded interface with smooth scrolling and high readability.
*   **System Health**: Real-time load averages and system uptime tracking.

## Building ProcX

To build ProcX from source, clone the repository and ensure you have the necessary dependencies.

```bash
git clone https://github.com/f0xptr/procx.git
cd procx
```

### Prerequisites

*   A C compiler (GCC or Clang)
*   `make` build tool
*   `ncurses` development libraries

**Debian/Ubuntu**:
```bash
sudo apt update && sudo apt install build-essential libncurses-dev
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
| `F1` | Sort by **PID** |
| `F3` | Sort by **CPU%** |
| `F4` | Sort by **Memory usage** |
| `F5` | Sort by **Process Name** |
| `/` | **Search** / Filter processes by name |
| `K` | **Kill** the selected process (requires confirmation) |
| `H` | Toggle **Help** overlay |
| `+` / `-` | Increase / Decrease refresh frequency |
| `Q` | Quit ProcX |

## Running Tests

ProcX includes unit tests for system information parsing:
```bash
make test
```

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for our workflow and [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for community guidelines.

## License

ProcX is open-source software licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
