// Copyright 2016, Jeffrey E. Bedard
#include "xstatus.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define XSTATUS_HELPTEXT \
	"DESCRIPTION:  Simple X toolbar for minimalistic"\
	" window managers.\n" \
	"USAGE: xstatus [-d DELAY][-f FILE][-h]\n" \
	"\t-d DELAY\tSet delay between status updates," \
	" in seconds.\n" \
	"\t-f FILE\t\tSet FILE to be continuously polled and" \
	" displayed.\n" \
	"\t-h\t\tPrint this usage information.\n" \
	"Copyright 2016, Jeffrey E. Bedard <jefbed@gmail.com>\n" \
	"Project page:  https://github.com/jefbed/xstatus\n"
__attribute__((noreturn))
static void usage(void)
{
	fputs(XSTATUS_HELPTEXT, stderr);
	exit(1);
}
static void parse_command_line(int argc, char ** argv,
	struct XStatusOptions * restrict o)
{
	int8_t opt;
	while((opt = getopt(argc, argv, "d:f:h")) != -1) {
		switch(opt) {
		case 'd':
			o->delay = atoi(optarg);
			break;
		case 'f':
			o->filename = optarg;
			break;
		case 'h':
		default:
			usage();
		}
	}
}
int main(int argc, char ** argv)
{
	struct XStatusOptions o = {.filename = XSTATUS_STATUS_FILE,
		.delay = 1};
	parse_command_line(argc, argv, &o);
	xstatus_start(&o);
}
