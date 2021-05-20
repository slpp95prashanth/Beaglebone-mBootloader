#ifndef _CMD_H
#define _CMD_H

#include <lib/string.h>

#define MAX_CMD_LEN 30
#define MAX_CMD_ARGS 6

#define CMD_CMP(str1, str2) strncmp(str1, str2, MAX_CMD_LEN)

void shell_start(void);

int do_md(int argc, char *argv[]);
int do_mm(int argc, char *argv[]);
int do_regdump(int argc, char *argv[]);
int do_reset(int argc, char *argv[]);
int do_eth_stats(void);
int do_gpio(int argc, char *argv[]);



#endif /* _CMD_H */
