#ifndef COLOR_H
#define COLOR_H

#include <QString>
#include <QByteArray>
#include <QtGui/QColor>
#include "types.h"

class Color {
public:
    Color();
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
    QString lighten(int percent, QString color = QString("")) const;
    QString darken(int percent, QString color = QString("")) const;
    QString saturate(int percent, QString color = QString("")) const;
    static QString c_saturate(float amount, QString color = QString(""));
    static QList<QString> saturateMultiple(QList<QString> &colors, float amount);
    static QString blendColor(QString &color, QString &otherColor);
private:
    QString hexToXRgba() const;
    QString saturateColor(double amount) const;
    yiq_t rgbToYiq(rgb_t &color);
    static QColor validateColorStr(QString &color);
};

#endif // COLOR_H
