#include "../include/appexception.h"
#include "../include/image.h"
#include "../include/logging.h"
#include "../include/util.h"
#include <QDirIterator>
#include <QRandomGenerator>


Walpp::Image::Image(QString &img_dir) {
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
QString Walpp::Image::getImage(QString &img, QString &cacheDir, bool iterative, bool recursive)
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
    Logging::info(QString("\033[1;31m%1\033[0m: %2").arg("image", logMessage));

    return wallpaper;
}

/**
 * Get all images in a directory recursively
 * @brief Image::getImagesRecursively
 * @return
 */
QStringList Walpp::Image::getImagesRecursively()
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
QStringList Walpp::Image::getAllImages()
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
QString Walpp::Image::getRandomImage(bool recursive)
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
QString Walpp::Image::getNextImage(bool recursive)
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

std::set<std::string> Walpp::Image::extractColours(std::string &imagePath, bool max, bool mode) {
    Magick::InitializeMagick(nullptr);
    Magick::Image image;

    try {
        // Read only first frame of multi-image files like GIFs
        image.read(imagePath + "[0]");
        // Check if image is valid
        if (!image.isValid()) {
            std::string message = "Invalid image: " + imagePath + " provided!";
            throw AppException(message);
        }
        if (!max) {
            // Resize to 25%
            image.resize(Magick::Geometry(image.columns() / 4, image.rows() / 4));

            // Reduce to 16 colors
            image.quantizeColors(16);
            image.quantize();
        }

        // Generate a set of unique colors
        Magick::Pixels view(image);
        uint width = image.columns();
        uint height = image.rows();
        std::set<std::string> uniqueColors;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                Magick::Color clr(image.pixelColor(j, i));
                // Format color to HEX
                float r = clr.quantumRed() * 255 / ((Magick::Quantum) 65535.0);
                float g = clr.quantumGreen() * 255 / ((Magick::Quantum) 65535.0);
                float b = clr.quantumBlue() * 255 / ((Magick::Quantum) 65535.0);

                std::ostringstream ss;
                if (mode) {
                    ss << "#" << std::hex << std::setw(2) << std::setfill('0') << r
                       << std::setw(2) << std::setfill('0') << g
                       << std::setw(2) << std::setfill('0') << b;
                } else {
                    ss << r << "," << g << "," << b;
                }
                uniqueColors.insert(ss.str());
            }
        }

        return uniqueColors;
    } catch (Magick::Exception &exception) {
        std::cerr << "Magick++ error: " << exception.what() << std::endl;
    }

    return {};
}

/**
 * Return
 * @param imagePath
 * @return
 */
int Walpp::Image::getPixelCount(std::string &imagePath) {
    Magick::Image image;
    image.read(imagePath);
    image.sample("256x256");
    int count  = image.rows() * image.columns();

    return count;
}
