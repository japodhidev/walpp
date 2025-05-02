#include "../include/appexception.h"
#include "../include/backend.h"
#include "../include/color.h"
#include "../include/util.h"

/**
 * Call Imagemagick to generate a scheme
 * @brief Wal::runImageMagick
 * @param colorCount
 * @param img
 * @param magickCmd
 * @return
 */
QByteArray Wal::runImageMagick(int colorCount, QString &img, QString &magickCmd) {
    return Util::checkOutput(magickCmd, QStringList() << QString("%1[0]").arg(img) << "-resize" << "25%" << "-colors" << QString("%1").arg(colorCount) << "-unique-colors" << "txt:-");
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
 * TODO: Generate maximum number of colors from image
 * Format the output from Imagemagick into a list of hex colors
 * @brief Wal::generateColors
 * @param img
 * @return
 */
QList<QString> Wal::generateColors(std::string &img) {
    QList<QString> colorList;
    QList<QByteArray> rawColors;
    QString magickCmd = hasIM();
    QString imgPath = QString::fromStdString(img);

    for (int i = 0; i < 20; i++) {
        rawColors = runImageMagick(16 + i, imgPath, magickCmd).split('\n');

        if (rawColors.size() > 16) {
            break;
        }

        if (i == 19) {
            std::string message = "Imagemagick couldn't generate a suitable palette.";
            throw AppException(message);
        } else {
            int count = 16 + i;
            qDebug() << "Imagemagick couldn't generate a palette.";
            qDebug() << QString("Trying a larger palette size %1").arg(count);
        }
    }

    if (rawColors.size() > 1) {
        QRegularExpression re(R"(#(?:[0-9a-fA-F]{3}){1,2})");
        foreach (const QByteArray &entry, rawColors) {
            if (!entry.startsWith("#")) {
                QString line(entry);
                QRegularExpressionMatch match = re.match(line);
                if (match.hasMatch()) {
                    auto hexColor = match.captured(0);
                    colorList.append(hexColor);
                }
            }

        }
    } else {
        qDebug() << "There was an error parsing the output from Imagemagick!";
    }
    return colorList;
}

/**
 * This is an overloaded function.
 * Generate maximum number of colors from an image.
 * Format the output from Imagemagick into a list of hex colors
 * @brief Wal::generateColors
 * @param img
 * @return
 */
QList<QString> Wal::generateColors(std::string &img, int count) {
    QList<QString> colorList;
    QList<QByteArray> rawColors;
    QString magickCmd = hasIM();
    QString imgPath = QString::fromStdString(img);

    rawColors = runImageMagick(count, imgPath, magickCmd).split('\n');

    if (rawColors.size() > 1) {
        QRegularExpression re(R"(#(?:[0-9a-fA-F]{3}){1,2})");
                foreach (const QByteArray &entry, rawColors) {
                if (!entry.startsWith("#")) {
                    QString line(entry);
                    QRegularExpressionMatch match = re.match(line);
                    if (match.hasMatch()) {
                        auto hexColor = match.captured(0);
                        colorList.append(hexColor);
                    }
                }

            }
    } else {
        qDebug() << "There was an error parsing the output from Imagemagick!";
    }
    return colorList;
}

/**
 * Adjust the generated colors and store them in a list
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
    auto slicedColors = colors.sliced(8);
    slicedColors.removeLast();
    foreach (const QString &entry, slicedColors) {
        rawColors.append(entry);
    }

    // Manually adjust colors
    Color color_o;
    if (light) {
        foreach (QString entry, rawColors) {
            entry = Color::c_saturate(0.5, entry);
        }
        rawColors.replace(0, color_o.lighten(0.85, colors.last()));
        rawColors.replace(7, colors.at(0));
        rawColors.replace(8,color_o.darken(0.4, colors.last()));
        rawColors.replace(15, colors.at(0));
    } else {
        // Darken the color slightly.
        if (!rawColors.at(1).startsWith("0")) {
            rawColors.replace(0, color_o.darken(0.4, rawColors.at(0)));
        }
        QString firstColor = rawColors.at(7);
        QString secondColor = "#EEEEEE";
        rawColors.replace(7, Color::blendColor(firstColor, secondColor));
        rawColors.replace(8, color_o.darken(0.33, rawColors.at(7)));
        QString thirdColor = rawColors.at(15);
        rawColors.replace(15, Color::blendColor(thirdColor, secondColor));
    }
    return rawColors;
}

QList<QString> Wal::get(QString &img, bool light) {
    std::string imgPath = img.toStdString();
    auto colors = generateColors(imgPath);

    return adjust(colors, light);
}
