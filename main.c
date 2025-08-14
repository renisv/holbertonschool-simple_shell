#include "shell.h"
/**
 * main - Entry point for the simple shell program.
 * @argc: The number of command-line arguments (unused).
 * @argv: Array of command-line argument strings.
 *
 * Description: This function implements a basic shell that:
 * - Runs in both interactive and non-interactive modes
 * - Reads commands from stdin
 * - Supports built-in commands 'exit' and 'env'
 * - Executes other commands using execve
 * - Maintains and returns the last execution status
 *
 * Return: The exit status of the last executed command.
 */
int main(int argc, char **argv)
{	char *line;
	size_t n;
	ssize_t r;
	int last_status;
	int interactive;
	(void)argc;
	line = NULL;
	n = 0;
	last_status = 0;
	interactive = isatty(STDIN_FILENO);
	while (1)
	{	char **args;
		if (interactive)
			write(STDOUT_FILENO, "($) ", 4);
		r = getline(&line, &n, stdin);
		if (r == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break; }
		args = split_line(line);
		if (!args)
			continue;
		if (!args[0])
		{	free_argv(args);
			continue; }
		if (strcmp(args[0], "exit") == 0)
		{	free_argv(args);
			free(line);
			exit(last_status); }
		else if (strcmp(args[0], "env") == 0)
		{	int i;
			for (i = 0; environ && environ[i]; i++)
			{
				write(STDOUT_FILENO, environ[i], strlen(environ[i]));
				write(STDOUT_FILENO, "\n", 1); }
			free_argv(args);
			continue; }
		execute_command(args, environ, argv[0] ? argv[0] : "hsh", &last_status);
		free_argv(args); }
	free(line);
	return (last_status); }
