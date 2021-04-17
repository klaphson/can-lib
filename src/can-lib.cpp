#include "can-lib.h"

CanBus::CanBus(const char* ifName, CanError_t& error) : mIfreq(), mAddr(), mFileDescriptor(0)
{
    error = eNoError;

    if (ifName != NULL)
    {
        size_t stringLength = strlen(ifName);

        if (stringLength <= IFNAMSIZ)
        {
            strcpy(mIfreq.ifr_ifrn.ifrn_name, ifName);
            Connect(error);
        }
        else
        {
            fprintf(stderr, "Given If name is too long!");
            error = eGivenIfNameTooLong;
        }
    }
    else
    {
        fprintf(stderr, "Given If name is NULL! ");
        error = eGivenIfNameIsNULL;
    }
}

CanBus::~CanBus(void)
{
    Disconnect();
}

void CanBus::Connect(CanError_t& error)
{
    error = eNoError;
    mFileDescriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (mFileDescriptor < 0)
    {
        perror("Can not create file descriptor! ");
        error = eCanNotCreateFileDescriptor;
    }
    else
    {
        ioctl(mFileDescriptor, SIOCGIFINDEX, &mIfreq);

        memset(&mAddr, 0, sizeof(mAddr));
        mAddr.can_family = AF_CAN;
        mAddr.can_ifindex = mIfreq.ifr_ifindex;

        if (bind(mFileDescriptor, (struct sockaddr*) &mAddr, sizeof(mAddr)) < 0)
        {
            perror("Can not bind socket! ");
            error = eCanNotBind;
        }
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

bool CanBus::SendPackage(const CanPackage& package, CanError_t& error) const
{
    error = eNoError;
    bool packageSent = false;
    const bool isConnected = IsConnected();

    if (isConnected)
    {
        struct can_frame frame = package.GetFrame();
        ssize_t bytesSent = write(mFileDescriptor, &frame, sizeof(struct can_frame));

        if (bytesSent != sizeof(struct can_frame))
        {
            perror("Error while writing! ");
            error = eCanNotWriteToFileDescriptor;
        }

        packageSent = true;
    }
    else
    {
        fprintf(stderr, "Connection is broken!");
    }

    return packageSent;
}

CanPackage CanBus::ReadPackage(CanError_t& error) const
{
    error = eUnknownError;
    CanPackage package;
    struct can_frame frame;

    ssize_t bytesRead = read(mFileDescriptor, &frame, sizeof(struct can_frame));

    if (bytesRead == sizeof(struct can_frame))
    {
        package = CanPackage(frame.can_id, frame.can_dlc, frame.data, error);
        error = eNoError;
    }
    else
    {
        if (bytesRead < 0)
        {
            perror("Error while reading! ");
            error = eCanNotReadFromFileDescriptor;
        }
        else if (bytesRead < sizeof(struct can_frame))
        {
            fprintf(stderr, "Incomplete CAN frame\n");
            error = eIncompleteFrameWasRead;
        }
        else
        {
        }
    }

    return package;
}