// Copyright 2017, Jeffrey E. Bedard
#include "xstatus.h"
#include "config.h"
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
    "Copyright 2017, Jeffrey E. Bedard <jefbed@gmail.com>\n" \
    "Project page:  https://github.com/jefbed/xstatus\n"
__attribute__((noreturn))
static void usage(void)
{
    write(1, XSTATUS_HELPTEXT, sizeof(XSTATUS_HELPTEXT));
    exit(1);
}
static int8_t next_option(int argc, char ** argv)
{
    return getopt(argc, argv, "d:f:h");
}
static void parse_command_line_r(int argc, char ** argv,
    struct XStatusOptions * restrict o, int8_t opt)
{
        switch(opt) {
        case -1:
            return;
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
        parse_command_line_r(argc, argv, o, next_option(argc, argv));
}
static void parse_command_line(int argc, char ** argv,
    struct XStatusOptions * restrict o)
{
    parse_command_line_r(argc, argv, o, next_option(argc, argv));
}
int main(int argc, char ** argv)
{
    struct XStatusOptions o = {.filename = XSTATUS_STATUS_FILE,
        .delay = 1};
    parse_command_line(argc, argv, &o);
    xstatus_start(&o);
}
