#include "can-lib.h"

CanBus::CanBus(const char* ifName) : mIfreq(), mAddr(), mFileDescriptor(0)
{
    if (ifName != NULL)
    {
        size_t stringLength = strlen(ifName);

        if (stringLength <= IFNAMSIZ)
        {
            strcpy(mIfreq.ifr_ifrn.ifrn_name, ifName);
        }
        else
        {
            perror("Given If name is too long! ");
            throw eGivenIfNameTooLong;
        }
    }
    else
    {
        perror("Given If name is NULL! ");
        throw eGivenIfNameIsNULL;
    }

    mFileDescriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (mFileDescriptor < 0)
    {
        perror("Can not create file descriptor! ");
        throw eCanNotCreateFileDescriptor;
    }

    ioctl(mFileDescriptor, SIOCGIFINDEX, &mIfreq);

    memset(&mAddr, 0, sizeof(mAddr));
    mAddr.can_family = AF_CAN;
    mAddr.can_ifindex = mIfreq.ifr_ifindex;

    if (bind(mFileDescriptor, (struct sockaddr*) &mAddr, sizeof(mAddr)) < 0)
    {
        perror("Can not bind socket! ");
        throw eCanNotBind;
    }
}

CanBus::~CanBus(void)
{
    close(mFileDescriptor);
}

bool CanBus::IsConnected(void) const
{
    bool isConnected = true;
    int32_t error = 0;
    socklen_t len = sizeof(error);
    int32_t retVal = getsockopt(mFileDescriptor, SOL_SOCKET, SO_ERROR, &error, &len);

    if ((retVal != 0) || (error != 0))
    {
        isConnected = false;
    }

    return isConnected;
}