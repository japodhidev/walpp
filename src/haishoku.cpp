#include "../include/haishoku.h"
#include "../include/HaishokuImage.h"
#include "../include/HaishokuAlgo.h"
#include <tuple>

Haishoku::Haishoku()  = default;

std::vector<ColorTuple> Haishoku::getColorsMean(std::string &imagePath) {
    auto colors = HaishokuImage::getColors(imagePath);
    auto sortedColors = HaishokuAlgo::sortByRGB(colors);
    auto groupedColors = HaishokuAlgo::groupByAccuracy(sortedColors);

    std::vector<ColorTuple> colorsMean;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                auto gColor = groupedColors[i][j][k];
                if (!gColor.empty()) {
                    colorsMean.push_back(HaishokuAlgo::getWeightedMean(gColor));
                }
            }
        }
    }

    // Python: sorted(colors_mean) uses default tuple compare — by count first,
    // then RGB. Then keep the last 8 (highest counts) and reverse-sort.
    std::sort(colorsMean.begin(), colorsMean.end());
    if (colorsMean.size() > 8) {
        colorsMean.erase(colorsMean.begin(), colorsMean.end() - 8);
    }
    std::sort(colorsMean.begin(), colorsMean.end(), std::greater<ColorTuple>{});

    return colorsMean;
}

std::vector<ColorTupleF> Haishoku::getPalette(std::string &imagePath) {
    auto colorsMean = getColorsMean(imagePath);

    int countSum = 0;
    for (const auto &c : colorsMean) {
        countSum += std::get<0>(c);
    }

    std::vector<ColorTupleF> plt;
    plt.reserve(colorsMean.size());
    for (const auto &c : colorsMean) {
        float weight = countSum > 0 ? static_cast<float>(std::get<0>(c)) / static_cast<float>(countSum) : 0.0f;
        plt.emplace_back(weight, std::get<1>(c));
    }

    return plt;
}

std::array<int, 3> Haishoku::getDominant(std::string &imagePath) {
    auto colorsMean = getColorsMean(imagePath);
    // colorsMean is already descending by count after getColorsMean, but keep
    // parity with the Python reference which re-sorts here defensively.
    std::sort(colorsMean.begin(), colorsMean.end(), std::greater<ColorTuple>{});
    return std::get<1>(colorsMean.at(0));
}

void Haishoku::showPalette(std::string &imagePath) {}
void Haishoku::showDominant(std::string &imagePath) {}

/**
 * Matches pywal/pywal/backends/haishoku.py:
 *   cols = [rgb_to_hex(col[1]) for col in Haishoku.getPalette(img)]
 *   cols.sort(key=util.rgb_to_yiq)
 *   raw = [*cols, *cols]
 *   raw[0] = util.lighten_color(cols[0], 0.40)
 *   return colors.generic_adjust(raw, light)
 */
std::vector<std::string> Haishoku::get(std::string &imagePath, bool light) {
    auto palette = getPalette(imagePath);

    std::vector<std::string> cols;
    cols.reserve(palette.size());
    for (const auto &entry : palette) {
        auto rgb = std::get<1>(entry);
        QColor qc = QColor::fromRgb(rgb[0], rgb[1], rgb[2]);
        cols.push_back(qc.name(QColor::HexRgb).toStdString());
    }

    auto yiqOf = [](const std::string &hex) {
        QColor c = QColor(QString::fromStdString(hex));
        rgb_t r{static_cast<double>(c.red()),
                static_cast<double>(c.green()),
                static_cast<double>(c.blue())};
        return Color::rgbToYiq(r);
    };

    std::sort(cols.begin(), cols.end(), [&](const std::string &a, const std::string &b) {
        yiq_t ya = yiqOf(a);
        yiq_t yb = yiqOf(b);
        return std::tie(ya.y_t, ya.i_t, ya.q_t) < std::tie(yb.y_t, yb.i_t, yb.q_t);
    });

    std::vector<std::string> raw;
    raw.reserve(16);
    raw.insert(raw.end(), cols.begin(), cols.end());
    raw.insert(raw.end(), cols.begin(), cols.end());
    // Pad if the palette had fewer than 8 unique colors (monochrome images).
    while (raw.size() < 16 && !cols.empty()) {
        raw.push_back(cols.back());
    }

    if (raw.empty()) {
        std::string message = "Haishoku produced an empty palette for the given image.";
        throw AppException(message);
    }

    raw.at(0) = Color::lightenColor(0.40f, cols.at(0));

    return Color::genericAdjust(raw, light);
}
