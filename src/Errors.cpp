//
// Created by DoktorovAlexander on 21.02.2026.
//

#include "Errors.hpp"

#ifdef WIN32
#define strerror_length(error) 100
#else
#define strerror_length(error) strerrorlen_s(errno) + 1
#endif


std::string plifaces::SystemErrorMessage() {
    const auto msgLen = strerror_length(errno);
    char errMessage[msgLen];
    strerror_s(errMessage, msgLen, errno);
    return { errMessage };
}