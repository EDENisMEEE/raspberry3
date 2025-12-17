# Bare-Metal Mini OS on Raspberry Pi3

This project is an experimental bare-metal mini operating system developed for Raspberry Pi3.
It is designed as a hands-on exercise to understand operating system fundamentals on ARM-based platforms,
without relying on an underlying OS or standard libraries. This work follows the [NYCU OSC 2024](https://nycu-caslab.github.io/OSC2024/) Labs.

## Features

### Lab1 Hello World
- Bare-metal execution (no Linux, no bootloader OS)
- Direct hardware access on Raspberry Pi3
- UART driver for serial output
- Simple shell

### Lab2 Booting
- [Bootloader](https://hackmd.io/Xgp3MVzlQCOwLUECpD8gPQ)
- [Initial Ramdisk](https://hackmd.io/j7uK7WlbR16HKFZkfeJKiQ)
- [Simple memory allocator](https://hackmd.io/MZbFHuhUQC-d8BgW6wxCsg)
- [Bootloader self-relocation](https://hackmd.io/n3EEbRmQT9yhS2kfo25JNw)
- [Device Tree]()


## Architecture Overview
- Target platform: Raspberry Pi (ARM architecture)
- Execution mode: Bare metal
- Programming languages: C and ARM assembly
- Cross-compilation using `arm-none-eabi-gcc`