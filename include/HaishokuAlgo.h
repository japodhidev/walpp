#ifndef WALPP_HAISHOKUALGO_H
#define WALPP_HAISHOKUALGO_H

#include "types.h"
#include <algorithm>
#include <cmath>

class HaishokuAlgo {
public:
    HaishokuAlgo();
    static std::vector<ColorTuple> sortByRGB(const std::vector<ColorTuple>& colors);
    static RGBMaxInfo rgbMaximum(const std::vector<ColorTuple>& colors);
    static GroupedColors groupByAccuracy(const std::vector<ColorTuple>& colors, int accuracy = 3);
    static ColorTuple getWeightedMean(const std::vector<ColorTuple>& group);
};

#endif //WALPP_HAISHOKUALGO_H
