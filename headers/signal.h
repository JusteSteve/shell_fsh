#ifndef SIGNAL_H
#define SIGNAL_H

#include "fsh.h"

extern volatile sig_atomic_t last_signal;
extern volatile sig_atomic_t signalint;

void handler_othersignals (int sig);
void handler_gather (int sig);

#endif // SIGNAL_H