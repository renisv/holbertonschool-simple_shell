#include "shell.h"


static char *_strdup_local(const char *s)
{
    size_t len;
    char *p;

    if (!s)
        return NULL;
    len = strlen(s);
    p = (char *)malloc(len + 1);
    if (!p)
        return NULL;
    memcpy(p, s, len + 1);
    return p;
}

char *find_env_value(char **env, const char *name)
{
    size_t nlen;
    int i;

    if (!env || !name)
        return NULL;

    nlen = strlen(name);
    for (i = 0; env[i]; i++)
    {
        if (!strncmp(env[i], name, nlen) && env[i][nlen] == '=')
            return env[i] + nlen + 1;
    }
    return NULL;
}


static char *join_path(const char *dir, const char *cmd)
{
    size_t len;
    char *out;

    len = strlen(dir) + 1 + strlen(cmd) + 1;
    out = (char *)malloc(len);
    if (!out)
        return NULL;

    out[0] = '\0';
    strcat(out, dir);
    strcat(out, "/");
    strcat(out, cmd);
    return out;
}

/*
 * resolve_command:
 *  - If cmd contains '/', check access(X_OK) and return a copy if executable.
 *  - Otherwise, search in PATH. Empty PATH entries map to "." per POSIX.
 *  - Returns malloc'ed full path or NULL if not found.
 */
char *resolve_command(char *cmd, char **env)
{
    char *path;
    char *norm;
    char *p;
    char *dir;
    char *full;
    size_t i, j, len;

    if (!cmd || !*cmd)
        return NULL;


    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return _strdup_local(cmd);
        return NULL;
    }

    path = find_env_value(env, "PATH");
    if (!path || !*path)
        return NULL;


    len = strlen(path);
    norm = (char *)malloc(len * 2 + 3);
    if (!norm)
        return NULL;

    j = 0;
    if (path[0] == ':')
    {
        norm[j++] = '.';
        norm[j++] = ':';
        i = 1;
    }
    else
    {
        i = 0;
    }

    while (i < len)
    {
        if (path[i] == ':')
        {
            norm[j++] = ':';
            if (i + 1 >= len || path[i + 1] == ':')
                norm[j++] = '.';
            i++;
        }
        else
        {
            norm[j++] = path[i++];
        }
    }
    if (j > 0 && norm[j - 1] == ':')
        norm[j++] = '.';

    norm[j] = '\0';

    
    p = strtok(norm, ":");
    while (p)
    {
        dir = p;
        full = join_path(dir, cmd);
        if (!full)
        {
            free(norm);
            return NULL;
        }
        if (access(full, X_OK) == 0)
        {
            free(norm);
            return full;
        }
        free(full);
        p = strtok(NULL, ":");
    }

    free(norm);
    return NULL;
}