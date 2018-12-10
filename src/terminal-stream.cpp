//
// Created by przemek on 12/8/18.
//

#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#include "../include/fdstream.h"

boost::fdostream get_ptmx_stream()
{
    char *slavename, window[64], buf[64];

    int master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }

    boost::fdostream ret(open(slavename, O_RDWR));
    return ret;
}

int get_ptmx()
{
    char *slavename, window[64], buf[64];

    int master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    int ret = open(slavename, O_RDWR);
    return ret;
}