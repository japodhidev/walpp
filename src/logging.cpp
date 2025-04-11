#include "../include/logging.h"

void Logging::info(QString message) {
    QTextStream out(stdout);
    QString name = "I";
    out << QString("[%1\033[0m]\033[1;31m\033[0m: %2").arg(name, message) << Qt::endl;
}
