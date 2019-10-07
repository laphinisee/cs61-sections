#include <unistd.h>
#include <cstdlib>
#include <cstring>

const char* messages[] = {
    "I remember the year after college I was broke.\n",
    "I realized I was becoming impossible, more and more impossible.\n",
    "Fear is overcome by procedure.\n",
    "When I make contact with a piece of paper without looking up I am happy.\n",
    "The mountain skies were clear except for the umlaut of a cloud.\n",
    "What's more, try it sometime. It works.\n"
};

void wait() {
    usleep(800000);
}

int main() {
    unsigned nmessages = sizeof(messages) / sizeof(messages[0]);

    while (true) {
        wait();

        const char* m = messages[rand() % nmessages];
        size_t n = write(1, m, strlen(m));
        (void) n;
    }
}
