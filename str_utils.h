#ifndef STR_UTILS_H
#define STR_UTILS_H
unsigned str_len(char*);
unsigned strn_len(char*, int);
unsigned str_cmp(char*, char*);
unsigned strn_cmp(char*, char*, int);
char* str_cpy(char*, char*);
char* strn_cpy(char*, char*, int);
char* str_dup(char*);
char* str_cat(char*, char*);
char* strn_cat(char*, char*, int);
int ends_with(char*, char*);
#endif
