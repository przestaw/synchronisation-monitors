//
// Created by przemek on 12/10/18.
//

#ifndef COCURRENT_TERMINAL_STREAM_H
#define COCURRENT_TERMINAL_STREAM_H

#include "fdstream.h"

boost::fdostream get_ptmx_stream();

int get_ptmx();

#endif //COCURRENT_TERMINAL_STREAM_H
