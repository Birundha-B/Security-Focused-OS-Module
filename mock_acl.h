#ifndef MOCK_ACL_H
#define MOCK_ACL_H

#include <string.h>

#define ALLOW 1
#define DENY 0

// Dummy Access Control for testing
int check_permission(const char *username, const char *filename, const char *action) {
    if (strcmp(username, "admin") == 0) return ALLOW;      // Admin has full access
    if (strcmp(action, "read") == 0) return ALLOW;        // Guest can read
    return DENY;                                          // Others denied
}

#endif
