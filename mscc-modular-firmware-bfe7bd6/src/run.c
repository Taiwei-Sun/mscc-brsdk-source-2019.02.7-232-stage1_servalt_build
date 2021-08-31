// Copyright (c) 2016 Microsemi Corporation "Microsemi".

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "mfi.h"

typedef enum
{
    false,
    true
} bool;

static bool is_executable_file(const char *n) {
    struct stat st;

    if (access(n, X_OK) != 0) return false;
    if (stat(n, &st) != 0) return false;
    if (!S_ISREG(st.st_mode)) return false;

    return true;
}

int try_run(const char *e) {
    static const char *exec_argv[] = {"", 0};
    static const char *exec_envp[] = {"HOME=/", "TERM=linux", "SHELL=/bin/sh",
                                      "USER=root", 0};

    debug("Try run: %s\n", e);
    if (!is_executable_file(e)) {
        debug("Try run: %s - not an executable\n", e);
        return -1;
    }

    exec_argv[0] = e;
    pid_t p = vfork();
    debug("Try run: %s - fork -> %d\n", e, p);

    if (p == -1) return -1;

    if (p == 0) {
        // Child code //////////////////////////////////////////////////////////
        int fd_max, i;

        for (i = 1; i < 32; ++i) signal(i, SIG_DFL);
        sigset_t set;
        sigfillset(&set);
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        if (setsid() == -1) perror("setsid: ");

        fd_max = sysconf(_SC_OPEN_MAX);
        if (fd_max == -1) fd_max = 1024;
        for (i = 0; i < fd_max; ++i) {
            if (i == STDIN_FILENO) continue;
            if (i == STDOUT_FILENO) continue;
            if (i == STDERR_FILENO) continue;
            close(i);
        }

        (void)execve(e, (char **)exec_argv, (char **)exec_envp);
        _exit(EXIT_FAILURE);
        // Child code end here /////////////////////////////////////////////////

    } else {
        int status;
        do {
            status = 0;
            waitpid(p, &status, 0);
            if (WIFEXITED(status)) {
                warn("Process %s (%d) returned with exit code %d\n", e, p,
                     WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                warn("Process %s (%d) was killed by signal %s (%d)\n", e, p,
                     strsignal(WTERMSIG(status)), WTERMSIG(status));
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 0;
}


