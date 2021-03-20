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

    for (uint8_t i = 0U; i < length; i++)
    {
        mCanPackage.data[i] = data[i];
    }
}

void CanPackage::Print(void) const
{
    printf("ID: %u [%u] ", mCanPackage.can_id, mCanPackage.can_dlc);

    for (uint8_t i = 0U; i < mCanPackage.can_dlc; i++)
    {
        printf("%02X ", mCanPackage.data[i]);
    }
}

const can_frame CanPackage::GetFrame(void) const
{
    return mCanPackage;
}