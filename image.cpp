#include "image.h"
#include "appexception.h"
#include <QDir>
#include <QDirIterator>
#include <QRandomGenerator>

Image::Image(QString &img_dir)
{
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
    return {};
}

/**
 * Get all images in a directory recursively
 * @brief Image::getImagesRecursively
 * @return
 */
QStringList Image::getImagesRecursively()
{
    QStringList images;
    QDirIterator dirIt(imgDir.absolutePath(), QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        QFileInfo f(dirIt.nextFileInfo());
        if (endsWithOneOf(f.fileName(), fileTypes)) {
            images.append(f.absolutePath());
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
        if (endsWithOneOf(entry.baseName(), fileTypes)) {
            images.append(entry.absoluteFilePath());
        }
    }

    return images;

    return {};
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
    QRandomGenerator prng(0, images.size());
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
    return {};
}

bool Image::endsWithOneOf(QString str, QStringList &items)
{
    QString t_table;
    foreach (const auto &entry, items) {
        bool hasValue = str.toLower().contains(entry);
        if (hasValue) {
            t_table.append("t");
        } else {
            t_table.append("f");
        }
    }

    return t_table.contains("t");
}
