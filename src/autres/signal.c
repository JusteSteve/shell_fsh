#include "../../headers/signal.h"
volatile sig_atomic_t signal_recu = 0;
volatile sig_atomic_t signal_sigint = 0;

void gestion_signal(int sig)
{
    switch(sig)
    {
        case SIGINT: // gestion de sigint
            //dprintf(2, "salut %d\n", sig);
            //signal_recu = 1;
            signal_sigint = 1;
            break;
        case SIGTERM: // 
            signal_recu = 1;
            //dprintf(2, "salut %d\n", sig);
            break;
        default:
            break;
    }
}

void init_signal(void)
{
    struct sigaction action = {0};
    action.sa_handler= gestion_signal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

void restaurer_signal(void)
{
    struct sigaction action = {0};
    action.sa_handler = SIG_DFL;
    sigaction(SIGINT, &action, NULL);
    //sigaction(SIGTERM, &action, NULL);

}