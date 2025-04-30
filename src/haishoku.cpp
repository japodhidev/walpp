#include "../include/haishoku.h"
#include "../include/HaishokuImage.h"
#include "../include/HaishokuAlgo.h"

Haishoku::Haishoku()  = default;

std::vector<ColorTuple> Haishoku::getColorsMean(std::string &imagePath) {
    // Get colors
    auto colors = HaishokuImage::getColors(imagePath);
    // Sort the image colors by RGB
    auto sortedColors = HaishokuAlgo::sortByRGB(colors);
    // Sort the colors by the accuracy
    auto groupedColors = HaishokuAlgo::groupByAccuracy(sortedColors);
    // Get the weighted mean of all the colors
    std::vector<ColorTuple> colorsMean;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                auto gColor = groupedColors[i][j][k];
                if (gColor.empty()) {
                    auto cMean = HaishokuAlgo::getWeightedMean(gColor);
                    colorsMean.push_back(cMean);
                }
            }
        }
    }
    // Return 8 of the most prevalent colors
    std::sort(colorsMean.begin(), colorsMean.end(), [](const ColorTuple &a, const ColorTuple &b) {
        return std::get<1>(a) < std::get<1>(b);
    });

    if (colorsMean.size() > 8) {
        // TODO: Extract & return the first 8 items
    }

    return colorsMean;
}

std::vector<std::tuple<float, int>> Haishoku::getPalette(std::string &imagePath) {
    auto colorsMean = this->getColorsMean(imagePath);
    std::vector<std::array<int, 3>> tempPalette;
    int countSum = 0;

    for (const auto &c : colorsMean) {
        countSum += std::get<0>(c);
        tempPalette.push_back(std::get<1>(c));
    }

    // Calculate the percentage
    std::vector<std::tuple<float, int>> palette;
    for (const auto &p : tempPalette) {
        auto rValue = (float)p[0] / (float)countSum;
        auto tempValue = std::make_tuple(rValue, p[1]);
        palette.push_back(tempValue);
    }

    return palette;
}

std::string Haishoku::getDominant(std::string &imagePath) {
    return "";
}