#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

int check_permission(const char *username, const char *filename, const char *action);
void list_permissions(const char *filename);

#endif
