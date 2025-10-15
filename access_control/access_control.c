#include <stdio.h>
#include <string.h>
#include <stddef.h>


#include "access_control.h"
#include "../user_management/user.h"

#define ACL_FILE "data/acl_table.txt"
// Function to check whether a user has permission for a given file and action
int check_permission(const char *username, const char *filename, const char *action) {
    FILE *file = fopen(ACL_FILE, "r");
    if (!file) {
        printf("⚠️ Error: acl_table.txt not found!\n");
        return 0;
    }

    User u = find_user(username);
    char line[256];
    int allowed = 0;

    while (fgets(line, sizeof(line), file)) {
        char fname[100], owner[50], roles[100], perms[10];

        // Format: filename owner allowed_roles permissions
        if (sscanf(line, "%s %s %s %s", fname, owner, roles, perms) == 4) {
            // match the filename
            if (strcmp(fname, filename) == 0) {
                // check if user's role is in the allowed roles list
                if (strstr(roles, u.role) != NULL) {
                    // check if permission type matches (R/W/E)
                    if (strchr(perms, action[0]) != NULL) {
                        allowed = 1;
                        break;
                    }
                }
            }
        }
    }

    fclose(file);
    return allowed;
}

// Function to display the file's permissions
void list_permissions(const char *filename) {
    FILE *file = fopen("../data/acl_table.txt", "r");
    if (!file) {
        printf("⚠️ Error: acl_table.txt not found!\n");
        return;
    }

    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char fname[100], owner[50], roles[100], perms[10];
        if (sscanf(line, "%s %s %s %s", fname, owner, roles, perms) == 4) {
            if (strcmp(fname, filename) == 0) {
                printf("\n📄 File: %s\n👤 Owner: %s\n🔑 Roles Allowed: %s\n🧩 Permissions: %s\n",
                       fname, owner, roles, perms);
                found = 1;
                break;
            }
        }
    }

    if (!found)
        printf("⚠️ File '%s' not found in ACL table.\n", filename);

    fclose(file);
}
