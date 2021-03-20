#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>


#include "can-package.h"

class CanBus
{
public:
    CanBus(const char* ifName);
    ~CanBus(void);

    bool IsConnected(void) const;

private:
    struct ifreq mIfreq;
    struct sockaddr_can mAddr;
    int32_t mFileDescriptor;
};