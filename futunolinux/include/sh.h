
#ifndef SH_H
# define SH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

/* ====== Globals ====== */
extern char *program_name;

/* ====== Limits / parse helpers ====== */
#define LINEBUF_MAX 2048
#define INIT_ARGV   8
#define IDENT_CHAR_P(c) (!isspace((int)(c)) && (c) != '|' && (c) != '>')

/* ====== Forward decls ====== */
typedef struct s_cmd     t_cmd;
typedef struct s_builtin t_builtin;

/* ====== Data structures ====== */
struct s_cmd {
    int             argc;
    char          **argv;
    int             capa;
    int             status;
    int             pid;
    struct s_cmd   *next;   /* ← struct s_cmd に修正 */
};

struct s_builtin {
    const char *name;
    int (*f)(int argc, char *argv[]);
};

/* ====== Builtins table (定義は .c に) ====== */
extern t_builtin builtins_list[];

/* ====== Pipeline helpers ====== */
#define PID_BUILTIN    (-2)
#define REDIRECT_P(node)  ((node)->argc == -1)
#define BUILTIN_P(node)   ((node)->pid == PID_BUILTIN)

/* HEAD_P は head との比較なので 2 引数にする */
#define HEAD_P(node, head) ((node) == (head))
#define TAIL_P(node)       (((node)->next == NULL) || REDIRECT_P((node)->next))

/* ====== API ====== */
void    prompt(void);
int     invoke_commands(t_cmd *cmd);             /* exec → wait のラッパ */
void    exec_pipeline(t_cmd *cmdhead);
int     wait_pipeline(t_cmd *cmdhead);
t_cmd  *pipeline_tail(t_cmd *cmdhead);
t_cmd  *parse_command_line(char *cmdline);
void    redirect_stdout(char *path);
void    free_cmd(t_cmd *p);
t_builtin *lookup_builtin(char *name);
int     builtin_cd(int argc, char *argv[]);
int     builtin_pwd(int argc, char *argv[]);
int     builtin_exit(int argc, char *argv[]);
void   *xmalloc(size_t sz);
void   *xrealloc(void *ptr, size_t sz);

#endif


// #ifndef SH_H
// # define SH_H


// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/wait.h>
// #include <fcntl.h>
// #include <limits.h>
// #include <ctype.h>
// #include <string.h>

// #define REDIRECT_P(t_cmd) ((cmd)->argc == -1)
// #define PID_BUILTIN -2
// #define BUILTIN_P(t_cmd) ((cmd)->pid == PID_BUILTIN)


// #define LINEBUF_MAX 2048

// // exec_pipeline
// #define HEAD_P(t_cmd) ((cmd) == cmdhead)
// #define TAIL_P(t_cmd) (((cmd)->next == NULL) || REDIRECT_P((cmd)->next))


// // parse_command
// #define INIT_ARGV 8
// #define IDENT_CHAR_P(c) (!isspace((int)c) && ((c) != '|') && ((c) != '>'))

// typedef struct s_cmd 
// {
//     int argc;
//     char **argv;
//     int capa;
//     int status;
//     int pid;
//     struct cmd *next;
// } t_cmd;

// typedef struct s_builtin 
// {
//     char *name;
//     int (*f)(int argc, char *argv[]);
// } t_builtin;


// t_builtin builtins_list[] = 
// {
//     {"cd",      builtin_cd},
//     {"pwd",     builtin_pwd},
//     {"exit",    builtin_exit},
//     {NULL,      NULL}
// };


// void prompt(void);
// int invoke_commands(struct cmd *cmd);
// void exec_pipeline(struct cmd *cmdhead);
// void redirect_stdout(char *path);
// int wait_pipeline(struct cmd *cmdhead);
// t_cmd *pipeline_tail(struct cmd *cmdhead);
// t_cmd *parse_command_line(char *cmdline);
// void free_cmd(struct cmd *p);
// t_builtin *lookup_builtin(char *name);
// int builtin_cd(int argc, char *argv[]);
// int builtin_pwd(int argc, char *argv[]);
// int builtin_exit(int argc, char *argv[]);
// void *xmalloc(size_t sz);
// void *xrealloc(void *ptr, size_t sz);


// #endif