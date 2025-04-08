#include "../include/color.h"
#include "../include/appexception.h"
#include <QDebug>
#include <string>
#include <QIODevice>
#include <QtGui/QColor>

Color::Color() = default;

Color::Color(QString &color) {    
    bool isValid = QColor::isValidColorName(color);

    if (!isValid) {
        std::string message = "Invalid HEX color string provided!";
        throw AppException(message);
    }
    this->walColor = QColor(color);
}

/**
 * Convert a hex color to rgb
 * @return
 */
QString Color::rgb() const {
    return QString("rgb(%1,%2,%3)")
        .arg(this->walColor.red())
        .arg(this->walColor.green())
        .arg(this->walColor.blue())
        ;
}

/**
 * Convert a hex color to xrdb rgba.
 * @return
 */
QString Color::xrgba() {
    try {
        QString xrgbaColor = hexToXRgba();

        return xrgbaColor;
    } catch (AppException &exception) {
        qDebug() << exception.errorMessage;
    }

    return {};
}

/**
 * Convert a hex color to rgba.
 * @return
 */
QString Color::rgba() const {
    return QString("rgba(%1,%2,%3,%4)")
        .arg(this->walColor.red())
        .arg(this->walColor.green())
        .arg(this->walColor.blue())
        .arg(this->walColor.alpha())
        ;
}

/**
 * Add URxvt alpha value to color.
 * @return
 */
QString Color::alpha() const {
    return QString("[%1]%2")
        .arg(alphaValue)
        .arg(this->walColor.name(QColor::HexRgb));
}

/**
 * Export the alpha value as a decimal number in [0, 1].
 * @return
 */
int Color::alphaDecimal() const {
    return (int) alphaValue / 100;
}

/**
 * Export color in decimal.
 * @return
 */
QString Color::decimal() const {
    bool ok = true;
    QString color = this->walColor.name(QColor::HexRgb);
    return QString("#%1").arg(color.removeFirst().toInt(&ok, 16));
}

/**
 * Strip '#' from decimal color.
 * @return
 */
int Color::decimalStrip() const {
    bool ok = true;
    QString color = this->walColor.name(QColor::HexRgb);
    return color.removeFirst().toInt(&ok, 16);
}

/**
 * Export color in octal.
 * @return
 */
QString Color::octal() const {
    bool ok;
    QString color = this->walColor.name(QColor::HexRgb);
    // Convert to hex first then to octal
    auto octalStr = QString::number(color.removeFirst().toInt(&ok, 16), 8);

    return QString("%1").arg(octalStr).prepend("#");
}

/**
 * Strip '#' from octal color.
 * @return
 */
QString Color::octalStrip() const {
    bool ok = true;
    QString color = this->walColor.name(QColor::HexRgb);
    // Convert to hex first then to octal
    auto octalStr = QString::number(color.removeFirst().toInt(&ok, 16), 8);

    return QString("%1").arg(octalStr);
}

/**
 * Strip '#' from color.
 * @return
 */
QString Color::strip() const {
    QString color = this->walColor.name(QColor::HexRgb);
    return color.removeFirst();
}

/**
 * Red value as float between 0 and 1.
 * @return
 */
QString Color::red() const {
    return QString("%1").arg(QString::number(this->walColor.red(), 'g', 3));
}

/**
 * Green value as float between 0 and 1.
 * @return
 */
QString Color::green() const {
    return QString("%1").arg(QString::number(this->walColor.green(), 'g', 3));
}

/**
 * Blue value as float between 0 and 1.
 * @return
 */
QString Color::blue() const {
    return QString("%1").arg(QString::number(this->walColor.blue(), 'g', 3));
}

/**
 * Lighten color by percent.
 * @return
 */
QString Color::lighten(int percent, QString color) const {
    int per100 = percent + 100;
    QColor lighterColor;

    if (color.isEmpty()) {
        lighterColor = this->walColor.lighter(per100);
    } else {
        QColor c = validateColorStr(color);
        lighterColor = c.darker(per100);
    }

    return lighterColor.name(QColor::HexRgb);
}

/**
 * Darken color by percent.
 * @return
 */
QString Color::darken(int percent, QString color) const {
    int per100 = (percent * 10) + 100;
    QColor darkerColor;

    if (color.isEmpty()) {
        darkerColor = this->walColor.darker(per100);
    } else {
        QColor c = validateColorStr(color);
        darkerColor = c.darker(per100);
    }

    return darkerColor.name(QColor::HexRgb);
}

/**
 * Saturate a color.
 * @return
 */
QString Color::saturate(int percent, QString color) const {
    double per100 = (double) percent / 100;
    hls_t hls{};
    if (color.isEmpty()) {
        hls.hue_t = this->walColor.hslHueF();
        hls.luminance_t = this->walColor.lightnessF();
    } else {
        QColor c = Color::validateColorStr(color);
        hls.hue_t = c.hslHueF();
        hls.luminance_t = c.lightnessF();
    }
    hls.saturation_t = (float)per100;

    QColor newRgbColors = QColor::fromHslF(hls.hue_t, hls.saturation_t, hls.luminance_t);

    return newRgbColors.name(QColor::HexRgb);
}

/**
 * Convert a hex color to xrdb rgba
 * @param color
 * @return
 */
QString Color::hexToXRgba() const {
    QString qStr = this->walColor.name(QColor::HexRgb);
    if (qStr.size() != 7) {
        std::string message = "Invalid HEX color string provided! Length mismatch.";
        throw AppException(message);
    }

    if (qStr.startsWith("#")) {
        qStr.removeFirst();
    }
    // Split string into pairs, seperated by a '/' & append '/ff'
    QString first = qStr.sliced(0, 2);
    QString second = qStr.sliced(2, 2);
    QString third = qStr.sliced(4, 2);

    return QString("%1/%2/%3/ff").arg(first, second, third);
}

/**
 * Blend two colors together.
 * @param color
 * @param otherColor
 * @return
 */
QString Color::blendColor(QString &color, QString &otherColor) {
    // Validation
    if (!QColor::isValidColorName(color) | !QColor::isValidColorName(otherColor)){
        std::string message = QString("Invalid HEX color string '%1' / '%2' provided!").arg(color, otherColor).toStdString();
        throw AppException(message);
    }

    auto c_color = QColor(color);
    auto o_color = QColor(otherColor);

    rgb_t newColor{};
    newColor.red_t = c_color.redF() * 0.5 + o_color.redF() * 0.5;
    newColor.green_t = c_color.greenF() * 0.5 + o_color.greenF() * 0.5;
    newColor.red_t = c_color.blueF() * 0.5 + o_color.blueF() * 0.5;

    QColor rColor = QColor::fromRgbF(newColor.red_t, newColor.green_t, newColor.blue_t);

    return rColor.name(QColor::HexRgb);
}

/**
 * Saturate a hex color
 * @param color
 * @param amount
 * @return
 */
QString Color::saturateColor(double amount) const {
    hls_t hls{};
    hls.hue_t = this->walColor.hslHueF();
    hls.luminance_t = this->walColor.lightnessF();
    hls.saturation_t = (float)amount;

    QColor newRgbColors = QColor::fromHslF(hls.hue_t, hls.saturation_t, hls.luminance_t);

    return newRgbColors.name(QColor::HexRgb);
}

/**
 * Convert an RGB color value to YIQ
 * @param color
 * @return
 */
yiq_t Color::rgbToYiq(rgb_t &color) {
    yiq_t result{};
    result.y_t = 0.30 * color.red_t + 0.59 * color.green_t + 0.11 * color.blue_t;
    result.i_t = 0.74 * (color.red_t - result.y_t) - 0.27 * (color.blue_t - result.y_t);
    result.q_t = 0.48 * (color.red_t - result.y_t) - 0.41 * (color.blue_t - result.y_t);

    return result;
}

QColor Color::validateColorStr(QString &color) {
    bool isValid = QColor::isValidColorName(color);

    if (!isValid) {
        std::string message = "Invalid HEX color string provided!";
        throw AppException(message);
    }

    return QColor{color};
}

/**
 * Saturate a list of colors by the provided amount
 * @brief Color::saturateMultiple
 * @param colors
 * @param amount
 * @return
 */
QList<QString> Color::saturateMultiple(QList<QString> &colors, float amount) {
    QList<QString> saturatedColors;
    foreach (QString color, colors) {
        Color c(color);
        QString sat_c = c.saturate((int) amount);
        saturatedColors.append(sat_c);
    }

    return saturatedColors;
}

/**
 * Saturate a hex color
 * @param amount
 * @param color
 * @return
 */
QString Color::c_saturate(float amount, QString color) {
    QColor colour = validateColorStr(color);
    float saturation = colour.saturationF() * (1 + amount);
    QColor hslColour = QColor::fromHslF(colour.hueF(), saturation, colour.lightnessF());

    return hslColour.name(QColor::HexRgb);
}
