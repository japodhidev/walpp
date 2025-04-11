#ifndef WALPP_LOGGING_H
#define WALPP_LOGGING_H

#include <QTextStream>
#include <QString>

class Logging {
public:
    static void info(QString message);
    static void error(QString message);
};

#endif //WALPP_LOGGING_H
