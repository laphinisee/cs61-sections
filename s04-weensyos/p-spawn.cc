#include "u-lib.hh"

void process_main() {
    pid_t alice_pid = sys_spawn("alice");

    unsigned i = 0;
    while (true) {
        ++i;
        if (i % 1024 == 0) {
            console_printf(0x0E00, "Hi, I'm Sheng! I started %d. #%d\n",
                           alice_pid, i / 512);
        }
        sys_yield();
    }
}
