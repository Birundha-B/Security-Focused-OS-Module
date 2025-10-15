#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>


#include "access_control.h"
#include "user.h"
#include "secure_read.h"
#include "secure_write.h"
#include "process.h"


#define LOG_FILE "logs/audit_log.txt"

void writeLog(const char *username, const char *action, const char *status) {
    FILE *logFile = fopen(LOG_FILE, "a");
    if (!logFile) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] User: %s | Action: %s | Status: %s\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec,
            username, action, status);
    fclose(logFile);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void add_new_user() {
    char username[50], password[50], role[20];
    printf("Enter username: "); scanf("%s", username);
    printf("Enter role (Admin/User/Guest): "); scanf("%s", role);
    printf("Enter password: "); scanf("%s", password);

    FILE *f = fopen("data/users.txt", "a");
    if (f) {
        fprintf(f, "%s:%s:%s\n", username, role, password);
        fclose(f);
        printf("✅ User added successfully.\n");
        writeLog(username, "Add New User", "Success");
    } else {
        printf("⚠️ Error adding user.\n");
    }
}

void add_new_file() {
    char filename[100], owner[50], roles[100], perms[10];
    printf("Enter filename: "); scanf("%s", filename);
    printf("Enter owner: "); scanf("%s", owner);
    printf("Enter allowed roles (comma separated): "); scanf("%s", roles);
    printf("Enter permissions (R/W/E): "); scanf("%s", perms);

    FILE *f = fopen("data/acl_table.txt", "a");
    if (f) {
        fprintf(f, "%s %s %s %s\n", filename, owner, roles, perms);
        fclose(f);
        printf("✅ File ACL entry added successfully.\n");
    } else {
        printf("⚠️ Error adding file ACL entry.\n");
    }
}

void add_new_process() {
    char process[100], roles[100];
    printf("Enter process name: "); scanf("%s", process);
    printf("Enter allowed roles (comma separated): "); scanf("%s", roles);

    FILE *f = fopen("process_security/process_table.txt", "a");
    if (f) {
        fprintf(f, "%s|%s\n", process, roles);
        fclose(f);
        printf("✅ Process added successfully.\n");
    } else {
        printf("⚠️ Error adding process.\n");
    }
}

int main() {
    int choice;
    char username[50], password[50], filename[100], data[256], process[100];
    int loggedIn = 0;
    User currentUser;

    clearScreen();
    printf("\n=== Security Focused OS CLI ===\n");

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. User Login\n");
        printf("2. Secure File Read\n");
        printf("3. Secure File Write\n");
        printf("4. Check Access Control\n");
        printf("5. View Process Table\n");
        printf("6. Execute Process\n");
        printf("7. Add New User\n");
        printf("8. Add New File (ACL Entry)\n");
        printf("9. Add New Process\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter Username: ");
                scanf("%s", username);
                printf("Enter Password: ");
                scanf("%s", password);

                currentUser = find_user(username);

                if (strcmp(currentUser.username, "unknown") != 0) {
                    if (strcmp(password, currentUser.password) == 0) {
                        loggedIn = 1;
                        printf("\nLogin successful. Welcome, %s! (Role: %s)\n", username, currentUser.role);
                        writeLog(username, "Login", "Success");
                    } else {
                        printf("\n❌ Invalid password.\n");
                        writeLog(username, "Login", "Failed");
                    }
                } else {
                    printf("\nUser not found. Do you want to create a new user? (y/n): ");
                    char ans; scanf(" %c", &ans);
                    if (ans == 'y' || ans == 'Y') {
                        char role[20], pass[50];
                        printf("Enter role (Admin/User/Guest): "); scanf("%s", role);
                        printf("Enter password: "); scanf("%s", pass);

                        FILE *f = fopen("data/users.txt", "a");
                        if (f) {
                            fprintf(f, "%s:%s:%s\n", username, role, pass);
                            fclose(f);
                            printf("✅ User created successfully. You are now logged in.\n");
                            strcpy(currentUser.username, username);
                            strcpy(currentUser.role, role);
                            strcpy(currentUser.password, pass);
                            loggedIn = 1;
                            writeLog(username, "New User Created & Login", "Success");
                        } else {
                            printf("⚠️ Error creating user.\n");
                        }
                    }
                }
                break;

            case 2:
                if (!loggedIn) { printf("Please login first.\n"); break; }
                printf("Enter filename to read: ");
                scanf("%s", filename);

                char *readData = secure_read(filename, username);
                if (readData) {
                    printf("File content: %s\n", readData);
                    free(readData);
                    writeLog(username, "Secure File Read", "Completed");
                } else {
                    writeLog(username, "Secure File Read", "Denied");
                }
                break;

            case 3:
                if (!loggedIn) { printf("Please login first.\n"); break; }
                printf("Enter filename to write: ");
                scanf("%s", filename);
                getchar();
                printf("Enter data to write: ");
                fgets(data, sizeof(data), stdin);
                data[strcspn(data, "\n")] = 0;

                if (secure_write(filename, data, username)) {
                    printf("File written successfully.\n");
                    writeLog(username, "Secure File Write", "Completed");
                } else {
                    printf("Write denied.\n");
                    writeLog(username, "Secure File Write", "Denied");
                }
                break;

            case 4:
                if (!loggedIn) { printf("Please login first.\n"); break; }
                printf("Enter filename to check: ");
                scanf("%s", filename);
                printf("Enter action (read/write/execute): ");
                char action[10];
                scanf("%s", action);

                if (check_permission(username, filename, action)) {
                    printf("%s access GRANTED for %s\n", action, filename);
                    writeLog(username, "Access Control Check", "Granted");
                } else {
                    printf("%s access DENIED for %s\n", action, filename);
                    writeLog(username, "Access Control Check", "Denied");
                }
                break;

            case 5:
                printf("\n--- Process Security Table ---\n");
                FILE *procFile = fopen("process_security/process_table.txt", "r");
                if (procFile) {
                    char line[200];
                    while (fgets(line, sizeof(line), procFile)) {
                        printf("%s", line);
                    }
                    fclose(procFile);
                } else {
                    printf("process_table.txt not found.\n");
                }
                writeLog("System", "View Process Table", "Displayed");
                break;

            case 6:
                if (!loggedIn) { printf("Please login first.\n"); break; }
                printf("Enter process to execute: ");
                scanf("%s", process);
                secure_exec(process, currentUser.role, username);
                break;

            case 7:
                if (!loggedIn) { printf("Please login first.\n"); break; }
                add_new_user();
                break;

            case 8:
                if (!loggedIn) { printf("Please login first.\n"); break; }
                add_new_file();
                break;

            case 9:
                if (!loggedIn) { printf("Please login first.\n"); break; }
                add_new_process();
                break;

            case 10:
                printf("\nExiting system. Goodbye!\n");
                writeLog("System", "Exit", "Program Terminated");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
