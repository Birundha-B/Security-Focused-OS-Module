#ifndef USER_H
#define USER_H

typedef struct {
    char username[50];
    char role[50];
    char password[50];
} User;

User find_user(const char *username);

#endif
