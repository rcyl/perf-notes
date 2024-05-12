#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    // Allocate memory using malloc. This does not allocate physical memory yet.
    
    size_t size = 1024 * 1024; // 1 MB of memory
    char *buffer = malloc(size);
    if (buffer == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }
    // Access each page in the buffer to trigger a page fault.
    // Typically, page size in Linux is 4096 bytes (4 KB).
    for (size_t i = 0; i < size; i += 4096) {
        buffer[i] = 0;
    }
    // printf("Memory allocated and initialized to trigger page faults.\n");
    // Free the allocated memory
    free(buffer);

    return 0;
}