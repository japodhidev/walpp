#include "../include/appexception.h"
#include <cstdlib>
#include <QTextStream>
#include <QString>

AppException::AppException(std::string &message) {
    errorMessage = message;
    Logging::exception(QString::fromStdString(errorMessage));
    std::exit(EXIT_FAILURE);
}
