// Copyright 2016, Jeffrey E. Bedard

#include "xstatus.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const char helptext[] =
"DESCRIPTION:  Simple X toolbar for minimalistic"
" window managers.\n"
"USAGE: xstatus [-d DELAY][-f FILE][-h]\n"
"\t-d DELAY\tSet delay between status updates,"
" in seconds.\n"
"\t-f FILE\t\tSet FILE to be continuously polled and"
" displayed.\n"
"\t-h\t\tPrint this usage information.\n"
"Copyright 2016, Jeffrey E. Bedard <jefbed@gmail.com>\n"
"Project page:  https://github.com/jefbed/xstatus\n";

int main(int argc, char ** argv)
{
	char *filename=XSTATUS_STATUS_FILE;
	uint8_t delay=1;
	int8_t opt;
	while((opt = getopt(argc, argv, "d:f:h")) != -1) {
		switch(opt) {
		case 'd':
			delay=atoi(optarg);
			break;
		case 'f':
			filename=optarg;
			break;
		case 'h':
		default:
			write(2, helptext, sizeof(helptext));
			exit(0);
		}
	}
	run_xstatus(filename, delay);
}

