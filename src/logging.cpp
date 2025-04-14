#include "../include/logging.h"

void Logging::info(QString message) {
    QTextStream out(stdout);
    QString name = "I";
    out << QString("[\033[1;33m%1\033[0m] %2").arg(name, message) << Qt::endl;
}

void Logging::error(QString message) {
    QTextStream out(stdout);
    QString name = "E";
    out << QString("[\033[1;33m%1\033[0m] %2").arg(name, message) << Qt::endl;
}
