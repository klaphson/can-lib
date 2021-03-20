#ifndef CAN_ERROR_H
#define CAN_ERROR_H

enum CanError_t
{
    eNoError,
    eCanPackageLentghTooBig,
    eGivenIfNameIsNULL,
    eGivenIfNameTooLong,
    eCanNotCreateFileDescriptor,
    eCanNotBind
};

#endif // CAN_ERROR_H