#include <stdio.h>
#include <string.h>
#include "../include/access_control.h"

int main() {
    char username[50], filename[50], action[10];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter filename: ");
    scanf("%s", filename);

    printf("Enter action (R/W/E): ");
    scanf("%s", action);

    if (check_permission(username, filename, action))
        printf("✅ ACCESS GRANTED: %s can perform %s on %s\n", username, action, filename);
    else
        printf("❌ ACCESS DENIED: %s cannot perform %s on %s\n", username, action, filename);

    printf("\nPermission details:\n");
    list_permissions(filename);

    return 0;
}
