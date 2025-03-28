#include "appexception.h"

AppException::AppException(std::string &message) {
    errorMessage = message;
}
