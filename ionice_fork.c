#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define	STDOUT_VERBOSE	0x01
#define	STDERR_VERBOSE	0x02

int
run_process(const char *path, char *argv[], int flags)
{
        pid_t pid;
        int rc, devnull_fd;
        char **argp;

        pid = vfork();
        if (pid == 0) {
                devnull_fd = open("/dev/null", O_WRONLY);

                if (devnull_fd < 0)
                        _exit(-1);

                if (!(flags & STDOUT_VERBOSE))
                        (void) dup2(devnull_fd, STDOUT_FILENO);

                if (!(flags & STDERR_VERBOSE))
                        (void) dup2(devnull_fd, STDERR_FILENO);

                close(devnull_fd);
#if 0
        { int i;
        fprintf(stderr, "Running process: ");
        for (i = 0; argv[i]; i++)
            fprintf(stderr, "'%s' ", argv[i]);
        fprintf(stderr, "\r\n");
        }
#endif
                setiopolicy_np(IOPOL_TYPE_DISK, IOPOL_SCOPE_PROCESS, IOPOL_THROTTLE);
                (void) execvp(path, argv);
		perror(path);
                _exit(-1);
        } else if (pid > 0) {
                int status;

                while ((rc = waitpid(pid, &status, 0)) == -1 &&
                        errno == EINTR);
                if (rc < 0 || !WIFEXITED(status))
                        return (-1);

                return (WEXITSTATUS(status));
        }

        return (-1);
}

int main(int argc, char *argv[])
{
        int c;

        if (argc < 2)
        {
                fprintf(stderr, "Usage: %s <command>\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        while ((c = getopt(argc, argv, "c:")) != -1);

        argc -= optind;
        argv += optind;

        setiopolicy_np(IOPOL_TYPE_DISK, IOPOL_SCOPE_PROCESS, IOPOL_THROTTLE);
        int err = run_process(argv[0], &argv[0], 0x03);
        exit (err);
}
