#ifndef COLOR_H
#define COLOR_H

#include "types.h"
#include <QString>
#include <QByteArray>
#include <QtGui/QColor>

class Color {
public:
    Color();
    explicit Color(QString &hexColor);
    explicit Color(std::string &hexColor);
    explicit Color(const std::string &hexColor);
    QColor walColor;
    double alphaValue = 100;

    QString rgb() const;
    QString xrgba();
    QString rgba() const;
    QString alpha() const;
    int alphaDecimal() const;
    QString decimal() const;
    int decimalStrip() const;
    QString octal() const;
    QString octalStrip() const;
    QString strip() const;
    QString red() const;
    QString green() const;
    QString blue() const;
    QString lighten(float amount, QString color = QString("")) const;
    std::string stdLighten(float amount, std::string color = "") const;
    QString darken(float amount, QString color = QString("")) const;
    std::string stdDarken(float amount, std::string color = "") const;
    QString saturate(float amount, QString color = QString("")) const;
    std::string stdSaturate(float amount, std::string color = "") const;
    static QString c_saturate(float amount, QString color = QString(""));
    static QList<QString> saturateMultiple(QList<QString> &colors, float amount);
    static QString blendColor(QString &color, QString &otherColor);
    static yiq_t rgbToYiq(rgb_t &color);
    static std::vector<std::string> genericAdjust(std::vector<std::string>, bool light = false);
private:
    QString hexToXRgba() const;
    QString saturateColor(double amount) const;
    static QColor validateColorStr(QString &color);
    static QColor validateColorStr(std::string &color);
};

#endif // COLOR_H
