#include <errno.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(void);

int main(int argc, char *argv[])
{
	long policy = 3;
	int ch;
	char *ep;

	//fake support for -c
	while ((ch = getopt(argc, argv, "c:")) != -1)
	{
		switch(ch)
		{
			case 'c':
				errno = 0;
				policy = strtol(optarg, &ep, 10);
				if (ep == optarg || *ep != '\0' || errno || policy < 0 || policy > 5)
					errx(1, "%s: invalid class/policy value", optarg);
				break;
			default:
				usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (argc == 0)
		usage();

	if (setiopolicy_np(IOPOL_TYPE_DISK, IOPOL_SCOPE_PROCESS, policy))
		warn("setiopolicy_np");
	
	execvp (argv[0], &argv[0]);
	int exit_status = errno ? EXIT_FAILURE : EXIT_SUCCESS;
	perror(argv[0]);
	exit (exit_status);
}

void usage(void)
{
	(void)fprintf(stderr, "usage: ionice [-c class] utility [argument ...]\n"
		"  -c\tClass or Policy [0-5]\n"
		"    \t    0=IOPOL_DEFAULT\n"
		"    \t    1=IOPOL_IMPORTANT\n"
		"    \t    2=IOPOL_PASSIVE\n"
		"    \t    3=IOPOL_THROTTLE (default)\n"
		"    \t    4=IOPOL_UTILITY\n"
		"    \t    5=IOPOL_STANDARD\n"
		"    \tsee man setiopolicy_np for more info\n");
	exit(1);
}
