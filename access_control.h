#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include "user.h"

int check_permission(const char *username, const char *filename, const char *action);
void list_permissions(const char *filename);

#endif
