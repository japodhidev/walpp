#ifndef WALPP_HAISHOKUIMAGE_H
#define WALPP_HAISHOKUIMAGE_H

#include "types.h"
#include <map>
#include <string>
#include <vector>
#include <Magick++.h>

class HaishokuImage {
public:
    HaishokuImage();
    Magick::Blob downloadImage(const std::string& url);
    Magick::Image getImage(const std::string& imagePath);
    Magick::Image getThumbnail(Magick::Image image);
    static std::vector<ColorTuple> getColors(const std::string& imagePath);
    Magick::Image newImage(const Magick::Geometry& size, const Magick::Color& color);
    void jointImage(const std::vector<Magick::Image>& images);
private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};


#endif //WALPP_HAISHOKUIMAGE_H
