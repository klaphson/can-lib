#ifndef CAN_PACKAGE_H
#define CAN_PACKAGE_H

#include <linux/can.h>

#include <stdint.h>
#include <stdio.h>

#include "can-error.h"

const uint8_t cMaximalDataLength = CAN_MAX_DLEN;

class CanPackage
{
public:
    CanPackage(const uint32_t id, const uint8_t size, const uint8_t* data);

    void Print(void) const;

    const can_frame GetFrame(void) const;

private:
    struct can_frame mCanPackage;
};

#endif // CAN_PACKAGE_H