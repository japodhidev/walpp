#include "../include/haishoku.h"
#include "../include/HaishokuImage.h"
#include "../include/HaishokuAlgo.h"
#include <cstdio>

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
                if (!gColor.empty()) {
                    auto cMean = HaishokuAlgo::getWeightedMean(gColor);
                    colorsMean.push_back(cMean);
                }
            }
        }
    }

    std::sort(colorsMean.begin(), colorsMean.end(), [](const ColorTuple &a, const ColorTuple &b) {
        return std::get<1>(a) < std::get<1>(b);
    });

    if (colorsMean.size() > 8) {
        // Extract & return the first 8 items
        colorsMean.resize(8);
    }

    // Reverse sort the colors
    std::sort(colorsMean.begin(), colorsMean.end(), [](const ColorTuple &a, const ColorTuple &b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    return colorsMean;
}

/**
 * @param imagePath
 */
std::vector<ColorTupleF> Haishoku::getPalette(std::string &imagePath) {
    auto colorsMean = getColorsMean(imagePath);
    std::vector<ColorTuple> tempPalette;
    int countSum = 0;

    for (const auto &c : colorsMean) {
        countSum += std::get<0>(c);
        // Append the entire tuple
        tempPalette.push_back(c);
    }

    // Calculate the percentage
    std::vector<ColorTupleF> plt;
    for (const auto &p : tempPalette) {
        auto rValue = (float)std::get<0>(p) / (float)countSum;
        auto tempValue = std::make_tuple(rValue, std::get<1>(p));
        plt.push_back(tempValue);
    }

    return plt;
}

std::array<int, 3> Haishoku::getDominant(std::string &imagePath) {
    auto colorsMean = getColorsMean(imagePath);
    // Sort in reverse order
    std::sort(colorsMean.begin(), colorsMean.end(), [](const ColorTuple &a, const ColorTuple &b){
        return std::get<1>(a) > std::get<1>(b);
    });
    // Get the dominant color
    auto dominantTuple = colorsMean.at(0);
    auto dom = std::get<1>(dominantTuple);

    return dom;
}

/**
 * FIXME: Implement
 * @param imagePath
 */
void Haishoku::showPalette(std::string &imagePath) {}

/**
 * FIXME: Implement
 * @param imagePath
 */
void Haishoku::showDominant(std::string &imagePath) {}


/**
 * Generate a list of colors extracted from the image in HEX format
 * @param imagePath
 * @param light
 * @return
 */
std::vector<std::string> Haishoku::get(std::string &imagePath, bool light) {
    auto colorPalette = getPalette(imagePath);
    std::vector<std::string> altResult;
    std::vector<std::string> rawColors;
    std::vector<yiq_t> yiqColors;
    Color c;
    bool test = true;
    for (const auto &ctpl : colorPalette) {
        auto rgbArray = std::get<1>(ctpl);
        rgb_t rgb = {};
        rgb.red_t = rgbArray[0];
        rgb.green_t = rgbArray[1];
        rgb.blue_t = rgbArray[2];
        auto yColor = Color::rgbToYiq(rgb);
        yiqColors.push_back(yColor);
        QColor color = QColor::fromRgb(rgb.red_t, rgb.green_t, rgb.blue_t);
        altResult.push_back(color.name(QColor::HexRgb).toStdString());
    }

    if (test) {
        // Sort the HEX colors by their YIQ values
        std::sort(yiqColors.begin(), yiqColors.end(), [](const yiq_t &a, const yiq_t &b) {
            // Convert yiq values to arrays and compare
            std::array<double, 3> first = {a.y_t, a.i_t, a.q_t};
            std::array<double, 3> second = {b.y_t, b.i_t, b.q_t};

            return first < second;
        });

        // Convert the sorted YIQ colors back to RGB
        std::vector<std::string> sortedHexColors;
        for (yiq_t entry : yiqColors) {
            rgb_t tempRgb = Color::yiqToRGB(entry);
            auto color = QColor::fromRgb(tempRgb.red_t, tempRgb.green_t, tempRgb.blue_t);
            sortedHexColors.push_back(color.name(QColor::HexRgb).toStdString());
        }
        rawColors.insert(rawColors.end(), sortedHexColors.cbegin(), sortedHexColors.cend());
        rawColors.insert(rawColors.end(), sortedHexColors.cbegin(), sortedHexColors.cend());
        // Lighten the first color by 0.40
        rawColors.at(0) = Color::lightenColor(0.40, sortedHexColors.at(0));
    } else {
        // Doubly append the list of sorted colors. The total should be 16 items
        rawColors.insert(rawColors.end(), altResult.cbegin(), altResult.cend());
        rawColors.insert(rawColors.end(), altResult.cbegin(), altResult.cend());    
        // Lighten the first color by 0.40
        rawColors.at(0) = Color::lightenColor(0.40, altResult.at(0));
    }
    
    assert(rawColors.size() >= 16);
    
    // Generic adjust
    auto colorList = Color::genericAdjust(rawColors, light);

    return colorList;
}
