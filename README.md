# ProcX

ProcX is a lightweight, ncurses-based process explorer and system monitoring tool for Linux. Written in C, it provides an aesthetic, real-time overview of running processes, CPU, memory, and swap usage directly in your terminal, offering an 'htop'-like experience.

## Features

*   Real-time display of active processes.
*   Monitoring of CPU, memory, and swap utilization.
*   Process details including PID, user, parent PID, threads, state, and memory usage.
*   Interactive UI with scrolling capabilities.

## Building ProcX

To build ProcX from source, you first need to clone the repository:

```bash
git clone https://github.com/f0xptr/procx.git
cd procx
```

Then, ensure you have a C compiler (like GCC) and the `ncurses` development libraries installed.

### Prerequisites

On Debian/Ubuntu-based systems:
```bash
sudo apt update
sudo apt install build-essential libncurses-dev
```

On Fedora/RHEL-based systems:
```bash
sudo dnf install gcc make ncurses-devel
```

### Compilation

Navigate to the root directory of the ProcX project and run `make`:
```bash
make
```

This will compile the source files and create the `procx` executable in the project root.

## Running ProcX

After successful compilation, you can run ProcX from your terminal:
```bash
./procx
```

### Controls

*   `q` or `Q`: Quit ProcX.
*   `Arrow Up`: Scroll up the process list.
*   `Arrow Down`: Scroll down the process list.

## Running Tests

The project includes a basic unit test for system information parsing. To run it:
```bash
make test
```

## Contributing

We welcome contributions to ProcX! Please see our [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute.

Please note that this project is released with a [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project, you agree to abide by its terms.

## License

ProcX is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
