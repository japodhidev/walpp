#ifndef WALPP_HAISHOKU_H
#define WALPP_HAISHOKU_H

#include "color.h"
#include "types.h"
#include "util.h"
#include <QColor>
#include <algorithm>
#include <string>
#include <vector>

class Haishoku {
public:
    Haishoku();
    std::vector<std::string> palette = {};
    std::array<int, 3> dominant;
    static std::vector<ColorTupleF> getPalette(std::string &imagePath);
    static std::vector<std::string> get(std::string &imagePath, bool light = false);
private:
    static std::vector<ColorTuple> getColorsMean(std::string &imagePath);
    void showPalette(std::string &imagePath);
    void showDominant(std::string &imagePath);
    std::array<int, 3> getDominant(std::string &imagePath);
};

#endif //WALPP_HAISHOKU_H
