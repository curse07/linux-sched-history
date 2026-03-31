Linux Kernel Scheduler History System Call
Overview

This project implements a custom Linux kernel system call, get_sched_history (assigned syscall number 451), designed to track and retrieve the scheduling behavior of processes. Developed for the Operating Systems [CS F372] course at BITS Pilani, KK Birla Goa Campus, this project involves direct modification of the Linux 5.19.8 kernel source to capture real-time context switch data.
Key Features

    Custom System Call (ID 451): A new interface to bridge kernel-space scheduling data to user-space applications.

    Real-time Event Logging: Captures critical scheduling data including nanosecond timestamps, CPU core allocation, and event types (Task IN/OUT).

    Kernel-Space Management: Implements a circular buffer within the kernel to store scheduling events, ensuring efficient memory usage and data persistence during process execution.

    Scheduler Integration: Deep integration with the core kernel scheduler (kernel/sched/core.c) to hook into every context switch.

System Call Implementation Details
1. Kernel-Space Logic (sched_syscall.c)

The core logic defines a sched_event structure to store event data:

    u64 timestamp: Time of the event in nanoseconds.

    int cpu: The CPU core where the process was scheduled.

    int event: Integer flag where 0 = IN and 1 = OUT.

    pid_t pid: The Process ID of the task.

The function log_sched_event is called by the scheduler to record these metrics into a 1000-entry global buffer.
2. Scheduler Hooks (core.c)

To ensure every switch is captured, the kernel's context_switch function was modified to call log_sched_event. This allows the system to record exactly when a process starts and stops its execution on a specific core.
3. Integration Files

    syscall_64.tbl: Registered get_sched_history as syscall 451 for the x86_64 architecture.

    Makefile: Updated the core kernel build system to include the new sched_syscall/ directory.

User-Space Verification (test.c)

A verification program was developed to demonstrate the system call's functionality. The program:

    Generates heavy CPU activity to trigger multiple context switches.

    Invokes syscall 451 to pull the recorded history from the kernel.

    Formats and prints the scheduling trail, including time-deltas and CPU migrations.

How to Build

    Prerequisites: Install kernel build dependencies (flex, bison, libssl-dev, libelf-dev, etc.).

    Configuration: Use make olddefconfig to prepare the .config file.

    Compilation: Compile using make -j$(nproc).

    Installation: Run make modules_install followed by make install and reboot into the new kernel.

Project Contributors

    Group 36 - KernelX 

    Jaikumar Wath (2023A3PS0197G) 

    Ayush Pareek (2023AAPS0609G) 

Developed as part of the Operating Systems course at BITS Pilani, Semester II, 2025–2026.
