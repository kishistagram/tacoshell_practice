#include "sh.h"

void redirect_stdout(char *path)
{ 
    int fd;

    close(1);
    fd = open(path, O_WRONLY|O_TRUNC|O_CREAT, 0666);
    if (fd < 0) {
        perror(path);
        return;
    }
    if (fd != 1) {
        dup2(fd, 1);
        close(fd);
    }
}