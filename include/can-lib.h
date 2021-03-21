#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>


#include "can-package.h"

class CanBus
{
public:
    CanBus(const char* ifName, CanError_t& error);
    ~CanBus(void);

    void Connect(CanError_t& error);
    void Disconnect(void);

    bool IsConnected(void) const;

    bool SendPackage(const CanPackage& package, CanError_t& error) const;

    CanPackage ReadPackage(CanError_t& error) const;

private:
    struct ifreq mIfreq;
    struct sockaddr_can mAddr;
    int32_t mFileDescriptor;
};