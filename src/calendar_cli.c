#include <stdio.h>
#include <string.h>
#include "calendar_cli.h"

typedef int (*cmd_f)(int argc, char *argv[]);

typedef struct cmd_entry_type {
    const char *name;
    cmd_f func
} cmd_entry_t;

static int cmd_days(int argc, char *argv[]) {
    return 0;
}

static int cmd_weekday(int argc, char *argv[]) {
    return 0;
}

static int cmd_print(int argc, char *argv[]) {
    return 0;
}

static int cmd_help(int argc, char *argv[]) {
    return 0;
}

static const cmd_entry_t cmd_table[] = {
    {"days", cmd_days},
    {"weekday", cmd_weekday},
    {"print", cmd_print},
    {"help", cmd_help}
};

#define CMD_TABLE_COUNT (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int exec_cmd(int argc, char *argv[]) {
    for(size_t i = 0; i < CMD_TABLE_COUNT; i++) {
        if(strcmp(argv[i], cmd_table[i].name) == 0)
            return cmd_table[i].func(argc, argv);
    }
    printf("Invalid command, please, enter '%s help' for more information.\n", argv[0]);
    return -1;
}

int calendar_cli(int argc, char *argv[]) {
    if(argv > 1)
        return exec_cmd(argc, argv);

    printf("Please, enter a valid command or '%s help' for more information.\n", argv[0]);
    return -1;
}