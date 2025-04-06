#include "../include/appexception.h"
#include <cstdlib>
#include <QTextStream>
#include <QString>

AppException::AppException(std::string &message) {
    errorMessage = message;
    QTextStream out(stdout);
    out << QString::fromStdString(errorMessage) << Qt::endl;
    std::exit(EXIT_FAILURE);
}
