#include "../include/appexception.h"
#include "../include/image.h"
#include "../include/logging.h"
#include "../include/util.h"
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

    QString path = Util::joinPath(cacheDir, QStringList() << "walpp");
    // Cache the image file path.
    Util::saveFile(wallpaper, path, false);

    QString logMessage = QString("Using image \033[1;37m%1\033[0m.").arg(wallpaper);
    Logging::info(logMessage);

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
        QString suffix = f.suffix();
        if (fileTypes.contains(suffix, Qt::CaseInsensitive)) {
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
        QString suffix = entry.suffix();
        if (fileTypes.contains(suffix, Qt::CaseInsensitive)) {
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
    auto rdx = QRandomGenerator::global()->bounded(0, images.size());
    return images.at(rdx);
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

