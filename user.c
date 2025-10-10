#include <stdio.h>
#include <string.h>
#include "../include/user.h"

// Function to find the user's role from users.txt
User find_user(const char *username) {
    FILE *file = fopen("../data/users.txt", "r");
    User u;

    // default values if user not found
    strcpy(u.username, "unknown");
    strcpy(u.role, "none");

    if (!file) {
        printf("⚠️ Error: users.txt not found!\n");
        return u;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char name[50], role[50];
        // Format: username:role
        if (sscanf(line, "%[^:]:%s", name, role) == 2) {
            if (strcmp(name, username) == 0) {
                strcpy(u.username, name);
                strcpy(u.role, role);
                break;
            }
        }
    }

    fclose(file);
    return u;
}
