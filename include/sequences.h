#ifndef SEQUENCES_H
#define SEQUENCES_H

#include <QMap>
#include "settings.h"
#include "types.h"

class Sequences
{
public:
    Sequences();
    static void send(QJsonObject &colors, QString cacheDir = Setting::CACHE_DIR, bool toSend = true, bool vteFix = false);
private:
    static QString setSpecial(int index,  QString color, const QString& itermName = "h", int alpha = 100);
    static QString setColor(int index, QString &hexColor);
    static QString setItermTabColor(rgb_t &color);
    static QString createSequences(QJsonObject &colors, bool vteFix = false);
    static QList<QString> findTerminals();
};

#endif // SEQUENCES_H
