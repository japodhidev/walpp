#ifndef BACKEND_H
#define BACKEND_H

class Wal {
 public:
     static QList<QString> get(QString &img, bool light = false);
     static QList<QString> generateColors(std::string &img);
     static QList<QString> generateColors(std::string &img, int count);
private:
    static QString hasIM();
    static QByteArray runImageMagick(int colorCount, QString &img, QString &magickCmd);
    static QList<QString> adjust(QList<QString> colors, bool light);
};

#endif // BACKEND_H
