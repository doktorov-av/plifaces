//
// Created by DoktorovAlexander on 21.02.2026.
//

#include "Errors.hpp"

#include <system_error>

#ifdef WIN32
#define strerror_length(error) 100
#else
#define strerror_length(error) strerrorlen_s(errno) + 1
#endif


std::string plifaces::SystemErrorMessage() {
    const auto errorCode = std::make_error_code(static_cast<std::errc>(errno));
    return errorCode.message();
}