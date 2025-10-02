#include "sh.h"


void exec_pipeline(t_cmd *cmdhead)
{
    t_cmd *cmd;
    int fds1[2] = {-1, -1};
    int fds2[2] = {-1, -1};

    for (cmd = cmdhead; cmd && !REDIRECT_P(cmd); cmd = cmd->next) 
    {
        fds1[0] = fds2[0];
        fds1[1] = fds2[1];
        if (! TAIL_P(cmd)) 
        {
            if (pipe(fds2) < 0) 
            {
                perror("pipe");
                exit(3);
            }
        }
        if (lookup_builtin(cmd->argv[0]) != NULL) {
            cmd->pid = PID_BUILTIN;
        }
        else {
            cmd->pid = fork();
            if (cmd->pid < 0) {
                perror("fork");
                exit(3);
            }
            if (cmd->pid > 0) 
            { /* parent */
                if (fds1[0] != -1) close(fds1[0]);
                if (fds1[1] != -1) close(fds1[1]);
                continue;
            }
        }
        if (! HEAD_P(cmd, cmdhead)) 
        {
            close(0); dup2(fds1[0], 0); close(fds1[0]);
            close(fds1[1]);
        }
        if (! TAIL_P(cmd)) 
        {
            close(fds2[0]);
            close(1); dup2(fds2[1], 1); close(fds2[1]);
        }
        if ((cmd->next != NULL) && REDIRECT_P(cmd->next)) 
        {
            redirect_stdout(cmd->next->argv[0]);
        }
        if (!BUILTIN_P(cmd)) 
        {
            execvp(cmd->argv[0], cmd->argv);
            fprintf(stderr, "%s: command not found: %s\n",
                    program_name, cmd->argv[0]);
            exit(1);
        }
    }
}


int wait_pipeline(t_cmd *cmdhead)
{
    t_cmd *cmd;

    for (cmd = cmdhead; cmd && !REDIRECT_P(cmd); cmd = cmd->next) {
        if (BUILTIN_P(cmd))
            cmd->status = lookup_builtin(cmd->argv[0])->f(cmd->argc, cmd->argv);
        else
            waitpid(cmd->pid, &cmd->status, 0);
    }
    return pipeline_tail(cmdhead)->status;
}

t_cmd *pipeline_tail(t_cmd *cmdhead)
{
    t_cmd *cmd;

    for (cmd = cmdhead; !TAIL_P(cmd); cmd = cmd->next)
        ;
    return cmd;
}