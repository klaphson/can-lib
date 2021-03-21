#include "can-package.h"

CanPackage::CanPackage(const uint32_t id, const uint8_t length, const uint8_t* data)
{
    mCanPackage.can_id = id;

    if (length <= cMaximalDataLength)
    {
        mCanPackage.can_dlc = length;
    }
    else
    {
        throw eCanPackageLentghTooBig;
    }

    memcpy(mCanPackage.data, data, length);
}

CanPackage::CanPackage(const CanPackage& package)
{
    mCanPackage.can_id = package.mCanPackage.can_id;
    mCanPackage.can_dlc = package.mCanPackage.can_dlc;
    memcpy(mCanPackage.data, package.mCanPackage.data, package.mCanPackage.can_dlc);
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

const can_frame CanPackage::GetFrame(void) const
{
    return mCanPackage;
}