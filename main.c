#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "sobel.h"

void f_usage(void)
{
	puts("usage:\n\t./fil --in in.txt --out out.txt --th 304 (rofl)");
}

int main(int argc, char *argv[])
{
	int th_num;
	char *path_i, *path_o;

	const struct option long_opt[] = {
		{"in",       1, 0, 'i' },
		{"out",      1, 0, 'o' },
		{"threads",  1, 0, 't' },
		{"help",     0, 0, 'h' },
		{0,          0, 0, 0   }
	};

	th_num = 0;
	path_i = path_o = NULL;

	while (1) {
		int c = getopt_long(argc, argv, "i:o:t:h", long_opt, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'i':
			path_i = optarg;
			break;
		case 'o':
			path_o = optarg;
			break;
		case 't':
			th_num = atoi(optarg);
			break;
		default:
			f_usage();
			exit(EXIT_SUCCESS);
		}
	}

	if (!path_i || !path_o || !th_num) {
		f_usage();
		exit(EXIT_SUCCESS);
	}

	sproc(path_i, path_o, th_num);

	exit(EXIT_SUCCESS);
}
