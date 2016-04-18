#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "xstatus.h"

static const char * helptext =
"DESCRIPTION:  Simple X toolbar for minimalistic window managers.  \n"
"USAGE: xstatus [-f FILE][-h]\n"
"\t-f FILE      Set FILE to be continuously polled and displayed.\n"
"\t-h           Print this usage information.  \n"
"Copyright 2016, Jeffrey E. Bedard <jefbed@gmail.com>\n"
"Project page:  https://github.com/jefbed/xstatus\n";

int main(int argc, char ** argv)
{
	static const char *filename=DEFAULTF;
	int opt;
	while((opt = getopt(argc, argv, "f:h")) != -1) {
		switch(opt) {
		case 'f':
			filename=optarg;
			break;
		case 'h':
		default:
			fputs(helptext, stderr);
		}
	}
	run_xstatus(filename);
}

