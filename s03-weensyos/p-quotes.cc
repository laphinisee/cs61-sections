#include "u-lib.hh"

const char* messages[] = {
    "I remember the year after college I was broke.\n",
    "I realized I was becoming impossible, more and more impossible.\n",
    "Fear is overcome by procedure.\n",
    "When I make contact with a piece of paper without looking up I am happy.\n",
    "The mountain skies were clear except for the umlaut of a cloud.\n",
    "What's more, try it sometime. It works.\n"
};

volatile unsigned counter;

void wait() {
    for (counter = 0; counter != 1U << 28; ++counter) {
    }
}

void process_main() {
    unsigned nmessages = sizeof(messages) / sizeof(messages[0]);

    while (true) {
        wait();

        const char* m = messages[rand(0, nmessages - 1)];
        sys_write(m, strlen(m));
    }
}
