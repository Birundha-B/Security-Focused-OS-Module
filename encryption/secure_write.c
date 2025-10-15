#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "../access_control/access_control.h"

int secure_write(const char *filename, const char *data, const char *username) {
    if (!check_permission(username, filename, "write")) {
        printf("Write Permission Denied for %s\n", username);
        return 0;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;

    for (int i = 0; i < strlen(data); i++)
        fputc(data[i] + 3, fp);  // Caesar Cipher (+3)

    fclose(fp);
    return 1;
}
