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

    void Connect(void);
    void Disconnect(void);

    bool IsConnected(void) const;

    bool SendPackage(const CanPackage& package) const;

private:
    struct ifreq mIfreq;
    struct sockaddr_can mAddr;
    int32_t mFileDescriptor;
};