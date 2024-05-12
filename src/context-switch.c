#include <unistd.h>
#include <stdio.h>

int main() {
    for (long i = 0; i < 10; i++) {
        sleep(1);
    }
    return 0;
}