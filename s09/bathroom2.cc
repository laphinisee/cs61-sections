#include <mutex>
#include <thread>
#include <cassert>
#include <cstdlib>
#define K 4

// Count the number of flushes per stall
unsigned nflushes[K];
void poop_into(int stall) {
    assert(stall >= 0 && stall < K);
    ++nflushes[stall];
}


// User action
void user() {
    int preferred_stall = random() % K;
    poop_into(preferred_stall);
}


// Thread function
void threadfunc() {
    for (int i = 0; i != 1000000; ++i) {
        user();
    }
}


int main(int argc, char** argv) {
    int nusers = 10;
    if (argc > 1) {
        nusers = strtol(argv[1], nullptr, 0);
    }
    assert(nusers > 0);

    std::thread* threads = new std::thread[nusers];
    for (int i = 0; i != nusers; ++i) {
        threads[i] = std::thread(threadfunc);
    }
    for (int i = 0; i != nusers; ++i) {
        threads[i].join();
    }
    delete[] threads;
}
