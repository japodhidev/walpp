#ifndef WALPP_HAISHOKU_H
#define WALPP_HAISHOKU_H

#include "types.h"
#include <string>
#include <vector>

class Haishoku {
public:
    Haishoku();
    std::vector<std::string> palette = {};
    std::string dominant;
    std::vector<ColorTuple> getColorsMean(std::string &imagePath);
    void showPalette(std::string &imagePath);
    void showDominant(std::string &imagePath);
    std::string getDominant(std::string &imagePath);
    std::vector<std::tuple<float, int>> getPalette(std::string &imagePath);
};

#endif //WALPP_HAISHOKU_H
