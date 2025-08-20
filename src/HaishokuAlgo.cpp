#include "../include/HaishokuAlgo.h"

HaishokuAlgo::HaishokuAlgo() = default;

ColorTuple HaishokuAlgo::getWeightedMean(const std::vector<ColorTuple> &group) {
    int sigmaCount = 0;
    long long sigma_r = 0, sigma_g = 0, sigma_b = 0;

    for (const auto &item : group) {
        int count = std::get<0>(item);
        auto rgb  = std::get<1>(item);
        sigmaCount += count;
        sigma_r += rgb[0] * count;
        sigma_g += rgb[1] * count;
        sigma_b += rgb[2] * count;
    }

    if (sigmaCount == 0) {
        return {0, {0, 0, 0}};
    }

    return {
        sigmaCount,
        {
            static_cast<int>(sigma_r / sigmaCount),
            static_cast<int>(sigma_g / sigmaCount),
            static_cast<int>(sigma_b / sigmaCount),
        }
    };
}

/**
 * @param colors
 * @return
 */
GroupedColors HaishokuAlgo::groupByAccuracy(const std::vector<ColorTuple> &colors) {
    GroupedColors rgbGroups{};
    RGBMaxInfo info = rgbMaximum(colors);

    for (const auto &color : colors) {
        int r = std::get<1>(color)[0];
        int g = std::get<1>(color)[1];
        int b = std::get<1>(color)[2];

        // int r_idx = (r < info.r_min + info.r_value) ? 0 : (r < info.r_min + 2 * info.r_value) ? 1 : 2;
        int r_idx = 2;
        if (r < (info.r_min + info.r_value)) {
            r_idx = 0;
        } else if (r < (info.r_min + info.r_value * 2)) {
            r_idx = 1;
        }
        int g_idx = (g < info.g_min + info.g_value) ? 0 : (r < info.g_min + info.g_value * 2) ? 1 : 2;
        int b_idx = (b < info.b_min + info.b_value) ? 0 : (r < info.b_min + info.b_value * 2) ? 1 : 2;
        rgbGroups[r_idx][g_idx][b_idx].push_back(color);
    }

    return rgbGroups;
}

RGBMaxInfo HaishokuAlgo::rgbMaximum(const std::vector<ColorTuple> &colors) {
    auto r_sorted = colors;
    auto g_sorted = colors;
    auto b_sorted = colors;

    // Sort the colors using the first (red) element
    std::sort(r_sorted.begin(), r_sorted.end(), [](const ColorTuple &a, const ColorTuple &b) {
        return std::get<1>(a)[0] < std::get<1>(b)[0];
    });

    // Sort the colors using the second (green) element
    std::sort(g_sorted.begin(), g_sorted.end(), [](const ColorTuple &a, const ColorTuple &b) {
        return std::get<1>(a)[1] < std::get<1>(b)[1];
    });

    // Sort the colors using the third (blue) element
    std::sort(b_sorted.begin(), b_sorted.end(), [](const ColorTuple &a, const ColorTuple &b) {
        return std::get<1>(a)[2] < std::get<1>(b)[2];
    });

    // Pick the minimum value for each color
    int r_min = std::get<1>(r_sorted.front())[0];
    int g_min = std::get<1>(g_sorted.front())[0];
    int b_min = std::get<1>(b_sorted.front())[0];

    // Pick the maximum value for each color
    int r_max = std::get<1>(r_sorted.back())[0];
    int g_max = std::get<1>(g_sorted.back())[0];
    int b_max = std::get<1>(b_sorted.back())[0];

    return {
        r_min, r_max,
        g_min, g_max,
        b_min, b_max,
        (r_max - r_min) / 3.0,
        (g_max - g_min) / 3.0,
        (b_max - b_min) / 3.0
    };
}

/**
 * Sort ColorTuple values by the second element (RGB)
 */
std::vector<ColorTuple> HaishokuAlgo::sortByRGB(const std::vector<ColorTuple> &colors) {
    std::vector<ColorTuple> sorted = colors;
    std::sort(sorted.begin(), sorted.end(), [](const ColorTuple &a, const ColorTuple &b) {
        return std::get<1>(a) < std::get<1>(b);
    });

    return sorted;
}