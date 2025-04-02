#ifndef TYPES_H
#define TYPES_H

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

#endif // TYPES_H
