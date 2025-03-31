#include "appexception.h"
#include <cstdlib>

AppException::AppException(std::string &message) {
    errorMessage = message;
    std::exit(EXIT_FAILURE);
}
