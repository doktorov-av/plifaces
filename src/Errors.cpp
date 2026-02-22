//
// Created by DoktorovAlexander on 21.02.2026.
//

#include "Errors.hpp"
#include <system_error>

std::string plifaces::SystemErrorMessage() {
    const auto errorCode = std::make_error_code(static_cast<std::errc>(errno));
    return errorCode.message();
}