#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QFileInfo>
#include <Magick++.h>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>

namespace Walpp {
    class Image
    {
    public:
        explicit Image(QString &img_dir);
        QFileInfo imgDir;
        QStringList fileTypes = {"png", "jpg", "jpeg", "jpe", "gif", "webp"};
        QString getImage(QString &img, QString &cacheDir, bool iterative, bool recursive);
        std::set<std::string> extractColours(std::string &imagePath);
    private:
        QStringList getImagesRecursively();
        QStringList getAllImages();
        QString getRandomImage(bool recursive);
        QString getNextImage(bool recursive);
    };
}

#endif // IMAGE_H
