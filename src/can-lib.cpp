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

    Connect();
}

CanBus::~CanBus(void)
{
    Disconnect();
}

void CanBus::Connect(void)
{
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

void CanBus::Disconnect(void)
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

bool CanBus::SendPackage(const CanPackage& package) const
{
    bool packageSent = false;
    const bool isConnected = IsConnected();

    if (isConnected)
    {
        struct can_frame frame = package.GetFrame();
        ssize_t bytesSent = write(mFileDescriptor, &frame, sizeof(struct can_frame));

        if (bytesSent != sizeof(struct can_frame))
        {
            perror("Error while writing ");
            throw eCanNotWriteToFileDescriptor;
        }

        packageSent = true;
    }
    else
    {
        perror("Connection is broken!");
    }

    return packageSent;
}

CanPackage CanBus::ReadPackage(void) const
{
    struct can_frame frame;

    ssize_t bytesRead = read(mFileDescriptor, &frame, sizeof(struct can_frame));

    if (bytesRead < 0)
    {
        perror("Error while reading ");
        throw eCanNotReadFromFileDescriptor;
    }

    if (bytesRead < sizeof(struct can_frame))
    {
        fprintf(stderr, "Incomplete CAN frame\n");
        throw eIncompleteFrameWasRead;
    }

    return CanPackage(frame.can_id, frame.can_dlc, frame.data);
}