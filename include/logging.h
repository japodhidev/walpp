#ifndef WALPP_LOGGING_H
#define WALPP_LOGGING_H

#include <QTextStream>
#include <QString>

class Logging {
public:
    static void info(const QString &message);
    static void error(const QString &message);
    static void exception(const QString &message);
};

#endif //WALPP_LOGGING_H
