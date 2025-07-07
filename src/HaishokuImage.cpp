#include "../include/HaishokuImage.h"

HaishokuImage::HaishokuImage()  = default;

Magick::Blob HaishokuImage::downloadImage(const std::string &url) {
    CURL *curl = curl_easy_init();
    std::vector<char> buffer;

    if (!curl) {
        std::string message = "CURL init failed!";
        throw AppException(message);
    }

    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::string message = "CURL download failed!";
        throw AppException(message);
    }

    return Magick::Blob(buffer.data(), buffer.size());
}

/**
 * Extract all colors from an image.
 * @param imagePath
 * @return
 */
std::vector<ColorTuple> HaishokuImage::getColors(std::string &imagePath) {
    auto colors = Util::extractMaxColoursPillow(imagePath);
    return colors;
}

Magick::Image HaishokuImage::getImage(const std::string &imagePath) {
    Magick::Image image;
    if (imagePath.rfind("http://", 0) == 0 || imagePath.rfind("https://", 0) == 0) {
        Magick::Blob imgBlob = downloadImage(imagePath);
        image.read(imgBlob);
    } else {
        image.read(imagePath);
    }

    if (image.colorSpace() != MagickCore::RGBColorspace) {
        image.colorSpace(MagickCore::RGBColorspace);
    }

    return image;
}

Magick::Image HaishokuImage::getThumbnail(Magick::Image image) {
    image.sample("256x256");
    return image;
}

[[maybe_unused]] void HaishokuImage::jointImage(const std::vector<Magick::Image> &images) {
    Magick::Geometry blockSize = images.front().size();
    size_t totalWidth = blockSize.width() * images.size();
    size_t height = blockSize.height();

    Magick::Image palette(Magick::Geometry(totalWidth, height), "white");
    palette.type(MagickCore::TrueColorType);

    size_t offset = 0;
    for (const auto &img : images) {
        palette.composite(img, Magick::Geometry(offset, 0), Magick::OverCompositeOp);
        offset += blockSize.width();
    }
    // TODO: display palette to terminal, definitely!
    // palette.display();
}

Magick::Image HaishokuImage::newImage(const Magick::Geometry &size, const Magick::Color &color) {
    Magick::Image img;
    img.size(size);
    img.backgroundColor(color);

    return img;
}

size_t HaishokuImage::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::vector<char>*)userp)->insert(((std::vector<char>*)userp)->end(), (char*) contents, (char*) contents + size + nmemb);

    return size + nmemb;
}