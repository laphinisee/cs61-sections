#ifndef WEENSYOS_PROCESS_HH
#define WEENSYOS_PROCESS_HH
#include "lib.hh"
#include "x86-64.h"
#if WEENSYOS_KERNEL
#error "process.hh should not be used by kernel code."
#endif

// process.h
//
//    Support code for WeensyOS processes.


// SYSTEM CALLS

// sys_getpid
//    Return current process ID.
inline pid_t sys_getpid() {
    register uintptr_t rax asm("rax") = SYSCALL_GETPID;
    asm volatile ("syscall"
                  : "+a" (rax)
                  :
                  : "cc", "rcx", "rdx", "rsi", "rdi",
                    "r8", "r9", "r10", "r11", "memory");
    return rax;
}

// sys_yield
//    Yield control of the CPU to the kernel. The kernel will pick another
//    process to run, if possible.
inline void sys_yield() {
    register uintptr_t rax asm("rax") = SYSCALL_YIELD;
    asm volatile ("syscall"
                  : "+a" (rax)
                  :
                  : "cc", "rcx", "rdx", "rsi", "rdi",
                    "r8", "r9", "r10", "r11", "memory");
}

// sys_write(ptr, len)
//    Write `len` bytes starting at `ptr` to the console.
inline int sys_write(const char* ptr, size_t len) {
    register uintptr_t rax asm("rax") = SYSCALL_WRITE;
    asm volatile ("syscall"
                  : "+a" (rax), "+D" (ptr), "+S" (len)
                  :
                  : "cc", "rcx", "rdx",
                    "r8", "r9", "r10", "r11", "memory");
    return rax;
}


// OTHER HELPER FUNCTIONS

// app_printf(format, ...)
//    Calls console_printf() (see lib.h). The cursor position is read from
//    `cursorpos`, a shared variable defined by the kernel, and written back
//    into that variable. The initial color is based on the current process ID.
void app_printf(int colorid, const char* format, ...);

#endif
