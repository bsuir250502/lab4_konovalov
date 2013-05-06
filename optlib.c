#include "optlib.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define OPTLIB_H 1

int getopt_c(char *arg, int hyphen_lim)
{                               /* hyphen = - */
    int arg_type = 0, i = 1;
    while (*arg == '-' && hyphen_lim--) {
        arg_type++;
        *(arg) = *(arg + 1);
        while (*(arg + i - 1)) {
            *(arg + i) = *(arg + i + 1);
            i++;
        }
        i = 1;
    }
    return arg_type;
}

int get_posint(int *target, FILE * stream)
{
    char buff[1000];
    int output = 0;
    char *check;
    printf("Maximum 9 digit number\n");
    fgets_c(buff, 1000, stream);
    *target = strtol(buff, &check, 10);
    if (*check != '\0' || strlen(buff) > 9 || *target < 0)
        output = -1;
    return output;
}

char fgets_c(char *target, int length, FILE * source)
{
    int i = 0;
    char buf[1000];
    fgets(buf, 1000, source);
    strncpy(target, buf, length - 1);
    target[length - 1] = '\0';
    if (strlen(buf) > length)
        printf
            ("String you entered was too long, data is partially lost.\n");
    while (target[i] != '\n' && target[i])
        i++;
    if (target[i] == '\n' && i)
        target[i] = '\0';
    return *target;
}
