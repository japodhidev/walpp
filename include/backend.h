#ifndef BACKEND_H
#define BACKEND_H

/*class Base {
public:
    static QList<QString> get(QString &img, bool light = false);
};*/

class Wal {
 public:
     static QList<QString> get(QString &img, bool light = false);
private:
    static QByteArray runImageMagick(int colorCount, QString &img, QString magickCmd);
    static QString hasIM();
    static QList<QString> generateColors(QString &img);
    static QList<QString> adjust(QList<QString> colors, bool light);
};

#endif // BACKEND_H
