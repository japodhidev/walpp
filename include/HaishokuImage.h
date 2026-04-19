#ifndef WALPP_HAISHOKUIMAGE_H
#define WALPP_HAISHOKUIMAGE_H

#include "appexception.h"
#include "types.h"
#include <string>
#include <vector>
#include <Magick++.h>

class HaishokuImage {
public:
    HaishokuImage();
    Magick::Image getImage(const std::string& imagePath);
    Magick::Image getThumbnail(Magick::Image image);
    static std::vector<ColorTuple> getColors(std::string& imagePath);
};


#endif //WALPP_HAISHOKUIMAGE_H
