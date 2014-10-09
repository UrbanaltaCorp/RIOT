// =============================================================================
// newlib_nano_write.c, required function when using newlib-nano & printf
// Chris Hodapp, 2014-04-08
// (c) 2014 Urbanalta Corp.
// =============================================================================
#include <errno.h>
#include <sys/unistd.h>

// Nordic-specific header:
#include "nrf_soc.h"

// For printf to do anything when we are using newlib-nano, we need to have a
// _write function; the code here is basically as described in:
// https://plus.google.com/+AndreyYurovsky/posts/XUr9VBPFDn7 
#undef errno
extern int errno;

int _write(int fd, char *ptr, int len)
{
    int res;

    switch (fd) {
    case STDOUT_FILENO:
    {
        int i;
        for (i = 0; i < len; i++) {

            NRF_UART0->TXD = (uint8_t) ptr[i];
            
            while (NRF_UART0->EVENTS_TXDRDY != 1);
            // Wait for TXD data to be sent...

            NRF_UART0->EVENTS_TXDRDY = 0;
        }
        res = len;
    }
    break;
 
    default:
        errno = EBADF;
        res = -1;
        break;
    }

    return res;
}
