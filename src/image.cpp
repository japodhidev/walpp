#include "../include/image.h"
#include "../include/appexception.h"
#include "../include/util.h"
#include <QDir>
#include <QDirIterator>
#include <QRandomGenerator>


Image::Image(QString &img_dir) {
    imgDir = QFileInfo(img_dir);
}

/**
 * Validate image input
 * @brief Image::getImage
 * @param img
 * @param cacheDir
 * @param iterative
 * @param recursive
 * @return
 */
QString Image::getImage(QString &img, QString &cacheDir, bool iterative, bool recursive)
{
    QString wallpaper;
    QFileInfo i(img);
    if (i.isFile()) {
        wallpaper = i.absoluteFilePath();
    } else if (i.isDir()) {
        if (iterative) {
            wallpaper = getNextImage(recursive);
        } else {
            wallpaper = getRandomImage(recursive);
        }
    } else {
        std::string message = "No valid image file found!";
        throw AppException(message);
    }

    QString name = i.fileName();
    QString path = Util::joinPath(cacheDir, QStringList() << name);
    Util::saveFile(wallpaper, path);

    QTextStream out(stdout);
    out << "Using image \033[1;37m" << wallpaper << "\033[0m.";

    return wallpaper;
}

/**
 * Get all images in a directory recursively
 * @brief Image::getImagesRecursively
 * @return
 */
QStringList Image::getImagesRecursively()
{
    QStringList images;
    QDirIterator dirIt(imgDir.absoluteFilePath(), QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        QFileInfo f(dirIt.nextFileInfo());
        QString fileName = f.fileName();
        if (endsWithOneOf(fileName, fileTypes)) {
            images.append(f.absoluteFilePath());
        }
    }

    return images;
}

/**
 * Get all images in a directory
 * @brief Image::getAllImages
 * @return
 */
QStringList Image::getAllImages()
{
    QStringList images;
    QFileInfo wallPath(imgDir);


    if (wallPath.dir().entryInfoList().empty()) {
        return {};
    }

    foreach (const auto &entry, wallPath.dir().entryInfoList()) {
        QString baseName = entry.baseName();
        if (endsWithOneOf(baseName, fileTypes)) {
            images.append(entry.absoluteFilePath());
        }
    }

    return images;
}

/**
 * Pick a random image file from a directory.
 * @brief Image::getRandomImage
 * @return
 */
QString Image::getRandomImage(bool recursive)
{
    QStringList images;
    if (recursive) {
        images = getImagesRecursively();
    } else {
        images = getAllImages();
    }

    if (images.empty()) {
        std::string message = "No images found in directory!";
        throw AppException(message);
    }

    // Pick a random image. Maybe similar to Python's random.shuffle()
    QRandomGenerator prng(nullptr, images.size());
    return images.at(prng.generate());
}

/**
 * Get next image in a dir
 * @brief Image::getNextImage
 * @param recursive
 * @return
 */
QString Image::getNextImage(bool recursive)
{
    QString wallpaper;
    QStringList images;
    if (recursive) {
        images = getImagesRecursively();
    } else {
        images = getAllImages();
    }

    if (!images.empty()) {
        if (images.size() >= 1) {
            wallpaper = images.takeAt(1);
        } else {
            wallpaper = images.takeAt(0);
        }
        images.pop_front();
    } else {
        std::string message = "No images found in directory!";
        throw AppException(message);
    }

    return recursive ?
               Util::joinPath(imgDir.absolutePath(), QStringList() << wallpaper) :
               Util::joinPath(imgDir.absolutePath(), QStringList() << "");
}

bool Image::endsWithOneOf(QString &eStr, QStringList &items)
{
    QString t_table;
    foreach (const auto &entry, items) {
        bool hasValue = eStr.toLower().contains(entry);
        if (hasValue) {
            t_table.append("t");
        } else {
            t_table.append("f");
        }
    }

    return t_table.contains("t");
}


