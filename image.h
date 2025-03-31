#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QFileInfo>

class Image
{
public:
    Image(QString &img_dir);
    QFileInfo imgDir;
    QStringList fileTypes = {".png", ".jpg", ".jpeg", ".jpe", ".gif", ".webp"};
    QString getImage(QString &img, QString &cacheDir, bool iterative, bool recursive);
private:
    QStringList getImagesRecursively();
    QStringList getAllImages();
    QString getRandomImage(bool recursive);
    QString getNextImage(bool recursive);
    bool endsWithOneOf(QString str, QStringList &items);
};

#endif // IMAGE_H
