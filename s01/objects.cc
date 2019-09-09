#include <cstdlib>
#include <cstdio>

char global_ch = 65;
const char const_global_ch = 66;

int main() {
    char local_ch = 67;
    char* allocated_ch = (char*) malloc(sizeof(char));
    *allocated_ch = 68;

    printf("Hello\n");
}
