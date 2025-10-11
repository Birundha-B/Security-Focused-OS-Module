#include <stdio.h>
#include <stdlib.h>
#include "secure_write.c"
#include "secure_read.c"

int main() {
    // Admin user writes and reads
    if (secure_write("test.txt", "Hello World", "admin")) {
        char *data = secure_read("test.txt", "admin");
        if (data) {
            printf("Admin Read: %s\n", data);  // Should print "Hello World"
            free(data);
        }
    }

    // Guest user write denied
    if (!secure_write("test.txt", "Hi User", "guest")) {
        printf("Guest write correctly denied\n");
    }

    // Guest user read allowed
    char *data = secure_read("test.txt", "guest");
    if (data) {
        printf("Guest Read: %s\n", data);  // Should print "Hello World"
        free(data);
    }

    return 0;
}
