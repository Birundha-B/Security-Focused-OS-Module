#include <stdio.h>
#include <string.h>
#include "user.h"

#define USERS_FILE "data/users.txt"

User find_user(const char *username) {
    FILE *file = fopen(USERS_FILE, "r");
    User u;
    strcpy(u.username, "unknown");
    strcpy(u.role, "none");
    strcpy(u.password, "");

    if (!file) {
        printf("⚠️ Error: users.txt not found!\n");
        return u;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char name[50], role[50], pass[50];
        if (sscanf(line, "%[^:]:%[^:]:%s", name, role, pass) == 3) {
            if (strcmp(name, username) == 0) {
                strcpy(u.username, name);
                strcpy(u.role, role);
                strcpy(u.password, pass);
                break;
            }
        }
    }

    fclose(file);
    return u;
}
