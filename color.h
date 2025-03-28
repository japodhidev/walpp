#ifndef COLOR_H
#define COLOR_H

#include <QString>
#include <QByteArray>
#include <QtGui/QColor>


struct rgb_t {
    double red_t;
    double green_t;
    double blue_t;
};

struct hls_t {
    float hue_t;
    float luminance_t;
    float saturation_t;
};

struct yiq_t {
    float y_t;
    float i_t;
    float q_t;
};

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
private:
    QString hexToXRgba() const;
    QString blendColor(QString &color, QString &otherColor);
    QString saturateColor(double amount) const;
    yiq_t rgbToYiq(rgb_t &color);
};

#endif // COLOR_H
