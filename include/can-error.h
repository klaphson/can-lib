#ifndef CAN_ERROR_H
#define CAN_ERROR_H

enum CanError_t
{
    eNoError,
    eUnknownError,
    eCanPackageLentghTooBig,
    eGivenIfNameIsNULL,
    eGivenIfNameTooLong,
    eCanNotCreateFileDescriptor,
    eCanNotBind,
    eCanNotWriteToFileDescriptor,
    eCanNotReadFromFileDescriptor,
    eIncompleteFrameWasRead
};

#endif // CAN_ERROR_H
