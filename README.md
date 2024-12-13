# "Real-time Systems Software (Zephyr RTOS)" Subject Labs

## Description

This project demonstrates the use of Zephyr RTOS for building and running applications on a native POSIX platform. It includes steps for setting up a Python virtual environment, building the application, and handling configuration files effectively.

## Requirements

- Zephyr RTOS installed and configured on your system.

- Python 3.6 or later.

- `west` tool for building and managing Zephyr projects.

- A POSIX-compliant operating system for the native build (Ubuntu 22.04 on VirtualBox is fine).

## Features

- Build and run applications using Zephyr RTOS on a native POSIX platform.

- Logging of build and execution output for debugging and analysis.

- Support for project-specific configurations with prj.conf files.

## Usage

### Setup project python virtual environment (.venv):

```bash
source ~/zephyrproject/.venv/bin/activate
```

### Build from `zephyrproject` directory (with logs):

```bash
west build -p auto -b native_posix my-zephyr-app -t run | tee log_output.txt
```

## Repetative Workflow

1. Activate the virtual python environment for Zephyr (performed from Zephyr root directory, once):

```bash
source ~/zephyrproject/.venv/bin/activate
```

2. Delete old `build` directory from Zephyr `root` directory (to delete old build configuration files left over from testing previous programs, if there were any);

3. Change the file `/my-zephyr-app/src/main.c` to the current code of the program being tested; optionally change the file `/my-zephyr-app/prj.conf` if you need to change the build configuration in accordance with `main.c` (add logging modules, timeslicing, etc.);

4. Build a new project with the current `main.c` and `prj.conf` files (saving the launch logs):

```bash
west build -p auto -b native_posix my-zephyr-app -t run | tee log_output.txt
```

5. Check the program output results and logs.

## Note

If there is a `prj.conf` file in a certain directory, it should be used with files in that directory instead of main `prj.conf` in the root directory (in order for the program to work properly).

## License

This project is developed for educational purposes. No explicit license is applied.
