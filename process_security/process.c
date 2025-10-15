#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stddef.h>


#define PROCESS_TABLE "process_security/process_table.txt"
#define LOG_FILE "logs/audit_log.txt"

void log_attempt(const char *username, const char *process, const char *status) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) return;

    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp)-1] = '\0';

    fprintf(log, "[%s] %s tried to execute %s → %s\n", timestamp, username, process, status);
    fclose(log);
}

int secure_exec(const char *process_name, const char *user_role, const char *username) {
    FILE *fp = fopen(PROCESS_TABLE, "r");
    if (!fp) {
        printf("Error opening process_table.txt\n");
        return 0;
    }

    char line[200];
    int allowed = 0;

    while (fgets(line, sizeof(line), fp)) {
        char process[100], roles[100];
        char *token = strtok(line, "|");
        if (token) strcpy(process, token);
        token = strtok(NULL, "\n");
        if (token) strcpy(roles, token);

        if (strcmp(process, process_name) == 0) {
            if (strstr(roles, user_role)) allowed = 1;
            break;
        }
    }

    fclose(fp);

    if (allowed) {
        printf("ACCESS GRANTED: %s is allowed to execute %s\n", username, process_name);
        log_attempt(username, process_name, "ALLOWED");
        return 1;
    } else {
        printf("ACCESS DENIED: %s cannot execute %s\n", username, process_name);
        log_attempt(username, process_name, "DENIED");
        return 0;
    }
}
