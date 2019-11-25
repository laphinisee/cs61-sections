#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdio>

/* G1 */

void a() {
    /* A1 */
    printf("1\n");
    /* A2 */
}

void b() {
    /* B1 */
    printf("2\n");
    /* B2 */
}

void c() {
    /* C1 */
    printf("3\n");
    /* C2 */
}

int main() {
    /* M1 */
    std::thread at(a);
    std::thread bt(b);
    std::thread ct(c);
    /* M2 */
    ct.join();
    bt.join();
    at.join();
}
