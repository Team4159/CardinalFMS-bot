#ifndef CHARHELPER_H
#define CHARHELPER_H

bool str_startsWith(const char *str, const char *pre);
bool str_equals(const char * str1, const char * str2);
char *str_replace(char *orig, char *rep, char *with);
char *str_concat(char *dest, char* src);

#endif