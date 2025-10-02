#include "sh.h"

// コマンドのフリー処理
void free_cmd(t_cmd *cmd)
{
    if (cmd->next != NULL)
        free_cmd(cmd->next);
    free(cmd->argv);
    free(cmd);
}

t_builtin builtins_list[] = 
{
    {"cd",      builtin_cd},
    {"pwd",     builtin_pwd},
    {"exit",    builtin_exit},
    {NULL,      NULL}
};


t_builtin *lookup_builtin(char *cmd)
{
    t_builtin *p;

    for (p = builtins_list; p->name; p++) 
    {
        if (strcmp(cmd, p->name) == 0)
            return p;
    }
    return NULL;
}

