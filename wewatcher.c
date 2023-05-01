#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    //FILE *log = fopen("./wewatcher.log", "w");
    if (argc != 2) {
        printf("Usage: %s <directory path>\n", argv[0]);
        return 1;
    }
    int kq;
    struct kevent event;
    struct timespec timeout = { 1, 0 };
    int fd;
    char *path = argv[1]; //"/Users/xim/Documents/test";
    pid_t pid;

    if ((fd = open(path, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if ((kq = kqueue()) == -1) {
        perror("kqueue");
        exit(EXIT_FAILURE);
    }

    EV_SET(&event, fd, EVFILT_VNODE,
           EV_ADD | EV_ENABLE | EV_CLEAR,
           NOTE_WRITE,
           0, NULL);

    if (kevent(kq, &event, 1, NULL, 0, &timeout) == -1) {
        perror("kevent");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            execlp("chmod", "chmod", "-R", "755", path, NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, NULL, 0);
            sleep(1);
        }

        if (kevent(kq, &event, 1, NULL, 0, &timeout) == -1) {
            perror("kevent");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}

