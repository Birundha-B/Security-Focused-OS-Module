#include <stdio.h>
#include <string.h>
#include <time.h>

// Function to log each process execution attempt
void log_attempt(char *username, char *process, char *status) {
    FILE *log = fopen("audit_log.txt", "a");
    if (log == NULL) {
        printf("Error opening audit_log.txt\n");
        return;
    }

    // Get current time
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp)-1] = '\0'; // Remove newline

    // Write log entry
    fprintf(log, "[%s] %s tried to execute %s → %s\n", timestamp, username, process, status);
    fclose(log);
}

// Function to check process permission
int secure_exec(char *process_name, char *user_role, char *username) {
    FILE *fp = fopen("process_table.txt", "r");
    if (fp == NULL) {
        printf("Error opening process_table.txt\n");
        return 0;
    }

    char line[200];
    int allowed = 0;

    while (fgets(line, sizeof(line), fp)) {
        char process[100], roles[100];
        // Split process and allowed roles
        char *token = strtok(line, "|");
        if (token != NULL)
            strcpy(process, token);

        token = strtok(NULL, "\n");
        if (token != NULL)
            strcpy(roles, token);

        // Check if process matches
        if (strcmp(process, process_name) == 0) {
            // Check if user role exists in allowed roles
            if (strstr(roles, user_role) != NULL)
                allowed = 1;
            break;
        }
    }

    fclose(fp);

    if (allowed) {
        printf("ACCESS GRANTED: %s is allowed to execute %s ✅\n", username, process_name);
        log_attempt(username, process_name, "ALLOWED");
        return 1;
    } else {
        printf("ACCESS DENIED: %s cannot execute %s ❌\n", username, process_name);
        log_attempt(username, process_name, "DENIED");
        return 0;
    }
}

// Main function to simulate
int main() {
    char username[50], role[50], process[100];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter role (Admin/User/Guest): ");
    scanf("%s", role);

    printf("Enter process to execute: ");
    scanf("%s", process);

    secure_exec(process, role, username);

    printf("\nCheck audit_log.txt for recorded attempts.\n");

    return 0;
}
