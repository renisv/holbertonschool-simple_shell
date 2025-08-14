#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;


char **split_line(char *line);
void free_argv(char **argv);


char *resolve_command(char *cmd, char **env);
char *find_env_value(char **env, const char *name);


int execute_command(char **argv, char **env, const char *progname, int *last_status);

#endif