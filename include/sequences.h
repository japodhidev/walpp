#ifndef SEQUENCES_H
#define SEQUENCES_H

#include <QMap>
#include "settings.h"
#include "types.h"

class Sequences
{
public:
    Sequences();
    void send(QMap<QString, QString> colors, QString cacheDir = Setting::CACHE_DIR, bool toSend = true, bool vteFix = false);
private:
    QString setSpecial(int index,  QString &color, QString itermName = "h", int alpha = 100);
    QString setColor(int index, QString &hexColor);
    QString setItermTabColor(rgb_t color);
    QString createSequence(QMap<QString, QString>, bool vteFix = false);
};

#endif // SEQUENCES_H
