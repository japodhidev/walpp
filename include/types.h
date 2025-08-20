#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <tuple>
#include <vector>

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
    double y_t;
    double i_t;
    double q_t;
};

using ColorTuple = std::tuple<int, std::array<int, 3>>; //(count, RGB)
using ColorTupleF = std::tuple<float, std::array<int, 3>>; //(count, RGB)
using GroupedColors = std::array<std::array<std::array<std::vector<ColorTuple>, 3>, 3>, 3>;
struct RGBMaxInfo {
    int r_min, r_max;
    int g_min, g_max;
    int b_min, b_max;
    double r_value, g_value, b_value;
};

#endif // TYPES_H
