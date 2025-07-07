#ifndef BACKEND_H
#define BACKEND_H

#include "appexception.h"
#include "color.h"
#include "util.h"
#include <vector>
#include <string>

class Wal {
 public:
     static QList<QString> get(QString &img, bool light = false);
     static QList<QString> generateColors(std::string &img);
     static QList<QString> generateColors(std::string &img, int count);
private:
    static QList<QString> adjust(QList<QString> colors, bool light);
};

#endif // BACKEND_H
