#include <stdio.h>
#include <stdlib.h>
#include "mock_acl.h"

// Secure read using Caesar Cipher (-3)
char* secure_read(const char *filename, const char *username) {
    if (!check_permission(username, filename, "read")) {
        printf("Read Permission Denied for %s\n", username);
        return NULL;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) return NULL;

    for (long i = 0; i < size; i++)
        buffer[i] = fgetc(fp) - 3;  // Caesar Cipher (-3)

    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}
