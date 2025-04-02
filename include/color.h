#ifndef COLOR_H
#define COLOR_H

#include <QString>
#include <QByteArray>
#include <QtGui/QColor>
#include "types.h"

class Color {
public:
    explicit Color(QString &hexColor);
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
    QString lighten(int percent) const;
    QString darken(int percent) const;
    QString saturate(int percent);
    static QList<QString> saturateMultiple(QList<QString> colors, float amount);
private:
    QString hexToXRgba() const;
    QString blendColor(QString &color, QString &otherColor);
    QString saturateColor(double amount) const;
    yiq_t rgbToYiq(rgb_t &color);
};

#endif // COLOR_H
