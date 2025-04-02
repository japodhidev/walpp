#ifndef BACKEND_H
#define BACKEND_H

class Base {
public:
    QJsonObject get(QString &img, bool light = false);
};

class Wal: Base {
// public:
//     QJsonObject get(QString &img, bool light = false);
private:
    void runImageMagick(int colorCount, QString &img, QStringList magickCmd);
    void hasIM();
    QList<String> generateColors(QString &img);
    QJsonObject adjust(QList<Qstring> colors, bool light);
};

#endif // BACKEND_H
