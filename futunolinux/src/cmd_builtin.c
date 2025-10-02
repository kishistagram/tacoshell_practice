#include "sh.h"

// ビルトインコマンドのcd
// chdir
int builtin_cd(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "%s: wrong argument\n", argv[0]);
        return 1;
    }
    if (chdir(argv[1]) < 0) {
        perror(argv[1]);
        return 1;
    }
    return 0;
}

// ビルトインコマンドのpwd
// getcwd()
int builtin_pwd(int argc, char *argv[])
{
    char buf[PATH_MAX];

    if (argc != 1) 
    {
        fprintf(stderr, "%s: wrong argument\n", argv[0]);
        return 1;
    }
    if (!getcwd(buf, PATH_MAX)) 
    {
        fprintf(stderr, "%s: cannot get working directory\n", argv[0]);
        return 1;
    }
    printf("%s\n", buf);
    return 0;
}

//　ビルトインコマンドのexit
int builtin_exit(int argc, char *argv[])
{
    if (argc != 1) 
    {
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        return 1;
    }
    exit(0);
}
