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
    double hue_t;
    double luminance_t;
    double saturation_t;
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

    QString rgb();
    QString xrgba();
    QString rgba();
    QString alpha() const;
    int alphaDecimal() const;
    QString decimal() const;
    int decimalStrip() const;
    QString octal() const;
    QString octalStrip() const;
    QString strip() const;
    QString red();
    QString green();
    QString blue();
    QString lighten(int percent);
    QString darken(int percent);
    QString saturate(int percent);
private:
    QString hexToXRgba(const std::string &color);
    QString blendColor(QString &color, QString &otherColor);
    QString saturateColor(double amount);
    yiq_t rgbToYiq(rgb_t &color);
};

#endif // COLOR_H
