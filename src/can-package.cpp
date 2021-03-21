#include "can-package.h"

CanPackage::CanPackage(
    const uint32_t id,
    const uint8_t length,
    const uint8_t* data,
    CanError_t& error)
{
    error = eNoError;
    mCanPackage.can_id = id;

    if (length <= cMaximalDataLength)
    {
        mCanPackage.can_dlc = length;
    }
    else
    {
        error = eCanPackageLentghTooBig;
    }

    memcpy(mCanPackage.data, data, length);
}

CanPackage::CanPackage(const CanPackage& package) : mCanPackage()
{
    mCanPackage.can_id = package.mCanPackage.can_id;
    mCanPackage.can_dlc = package.mCanPackage.can_dlc;
    memcpy(mCanPackage.data, package.mCanPackage.data, package.mCanPackage.can_dlc);
}

CanPackage::CanPackage(void) : mCanPackage()
{
}

CanPackage& CanPackage::operator=(const CanPackage& package)
{
    if (this != &package)
    {
        mCanPackage.can_id = package.mCanPackage.can_id;
        mCanPackage.can_dlc = package.mCanPackage.can_dlc;
        memcpy(mCanPackage.data, package.mCanPackage.data, package.mCanPackage.can_dlc);
    }

    return *this;
}

void CanPackage::Print(void) const
{
    printf("ID: %X [%X] ", mCanPackage.can_id, mCanPackage.can_dlc);

    for (uint8_t i = 0U; i < mCanPackage.can_dlc; i++)
    {
        printf("%02X ", mCanPackage.data[i]);
    }

    printf("\n");
}

can_frame CanPackage::GetFrame(void) const
{
    return mCanPackage;
}