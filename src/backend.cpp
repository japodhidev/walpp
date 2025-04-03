#include <utility>

#include "../include/util.h"
#include "../include/appexception.h"
#include "../include/backend.h"
#include "../include/color.h"

/**
 * Call Imagemagick to generate a scheme
 * @brief Wal::runImageMagick
 * @param colorCount
 * @param img
 * @param magickCmd
 * @return
 */
QByteArray Wal::runImageMagick(int colorCount, QString &img, QString magickCmd) {
    Util::checkOutput(std::move(magickCmd), QStringList() << QString("%1[0]").arg(img) << "-resize" << "25%" << "-colors" << QString("").arg(colorCount) << "-unique-colors" << "txt:-");
}

/**
 * Check to see if the user has Imagemagick installed
 * @brief Wal::hasIM
 * @return
 */
QString Wal::hasIM() {
    QString prog = "magick";
    if (Util::which(prog).contains("magick")) {
        return prog;
    }

    std::string message = "Imagemagick wasn't found on your system. Try another backend. (wal --backend)";
    throw AppException(message);
}

/**
 * Format the output from Imagemagick into a list of hex colors
 * @brief Wal::generateColors
 * @param img
 * @return
 */
QList<QString> Wal::generateColors(QString &img) {
    QList<QString> colorList;
    QList<QByteArray> rawColors;
    QString magickCmd = hasIM();

    for (int i = 0; i <= 20; i++) {
        rawColors = runImageMagick(16 + i, img, magickCmd).split('\n');

        if (rawColors.size() > 16) {
            break;
        }

        if (i == 19) {
            std::string message = "Imagemagick couldn't generate a suitable palette.";
            throw AppException(message);
        } else {
            qDebug() << "Imagemagick couldn't generate a palette.";
            qDebug() << QString("Trying a larger palette size %1").arg(16 + i);
        }
    }

    if (rawColors.size() > 1) {
        foreach (const QByteArray &entry, rawColors) {
            auto hexColor = entry.split(' ').at(4);
            QString hexColorStr(hexColor);
            if (hexColorStr.startsWith("#")) {
                colorList.append(hexColorStr);
            }
        }
    } else {
        qDebug() << "There was an error parsing the output from Imagemagick!";
    }

    return colorList;
}

/**
 * Adjust the generated colors and store them in a JsonObject
 * @brief Wal::adjust
 * @param colors
 * @param light
 * @return
 */
QList<QString> Wal::adjust(QList<QString> colors, bool light) {
    QList<QString> rawColors;
    // The first color
    rawColors.append(colors.at(0));
    // Slice starting from the item at index 8, count 8 items
    foreach (const QString &entry, colors.sliced(8, 8)) {
        rawColors.append(entry);
    }
    // Slice starting from the item at index 8 to the item before last
    foreach (const QString &entry, colors.sliced(8, colors.size() - 1)) {
        rawColors.append(entry);
    }

    // Manually adjust colors
    Color color_o;
    if (light) {
        foreach (QString entry, rawColors) {
            entry = color_o.saturate(50, entry);
        }
        rawColors.replace(0, color_o.lighten(85, colors.last()));
        rawColors.replace(7, colors.at(0));
        rawColors.replace(8,color_o.darken(40, colors.last()));
        rawColors.replace(15, colors.at(0));
    } else {
        // Darken the color slightly.
        if (!rawColors.at(1).startsWith("0")) {
            rawColors.replace(0, color_o.darken(40, rawColors.at(0)));
        }
        rawColors.replace(7, Color::blendColor(const_cast<QString &>(rawColors.at(7)), (QString &) "#EEEEEE"));
        rawColors.replace(8, color_o.darken(30, rawColors.at(7)));
        rawColors.replace(15, Color::blendColor(const_cast<QString &>(rawColors.at(15)), (QString &) "#EEEEEE"));
    }

    return rawColors;
}

QList<QString> Wal::get(QString &img, bool light) {
    auto colors = generateColors(img);

    return adjust(colors, light);
}
