// Copyright 2017, Jeffrey E. Bedard
#include "util.h"
#include "font.h"
#include "libjb/file.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
// returns -1 on error
int xstatus_system_value(const char * filename)
{
	ssize_t sz = 8;
	char buf[sz];
	fd_t f = open(filename, O_RDONLY);
	sz = read(f, buf, sz); // Read if we can
	close(f); // Clean up
	// This check is all-inclusive:
	return sz > 0 ? atoi(buf) : -1;
}
