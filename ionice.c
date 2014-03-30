#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int c;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//fake support for -c
	while ((c = getopt(argc, argv, "c:")) != -1);

	argc -= optind;
	argv += optind;

	setiopolicy_np(IOPOL_TYPE_DISK, IOPOL_SCOPE_PROCESS, IOPOL_THROTTLE);
	execvp (argv[0], &argv[0]);
	int exit_status = errno ? EXIT_FAILURE : EXIT_SUCCESS;
	perror(argv[0]);
	exit (exit_status);
}
