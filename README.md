# Voi
Application firewall with filtering performed in the kernel, for Linux.

## Status
- Currently just scaffolding code
- No where near ready for a production system.
- Still only has hardcoded test matches for processes.

## Why?
Originally just an excercise in netfilter hooking, a Linux-capable copy of [Lulu](https://objective-see.com/products/lulu.html) with the filtering done inside the kernel would be nice.

## Building the Kernel module
The kernel module code compiles cleanly on latest kernels. Make sure you have the kernel headers for your current running kernel installed.
```
cd kernel
make
sudo make insert
```

## Building the Userspace utilities
The current userspace tools are basically just netlink PoC tests.
```
cd userspace
clang -o setpid setpid_and_listen.c -lnetlink
```
