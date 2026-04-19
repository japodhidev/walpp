#include "../include/HaishokuImage.h"
#include <algorithm>
#include <cstdint>
#include <unordered_map>

HaishokuImage::HaishokuImage() = default;

Magick::Image HaishokuImage::getImage(const std::string &imagePath) {
    Magick::Image image;
    image.read(imagePath);
    // Flatten any alpha channel onto white before quantizing, matching PIL's
    // Image.convert("RGB") behaviour which drops alpha via compositing.
    image.backgroundColor(Magick::Color("white"));
    image.alpha(false);
    // Strip embedded ICC/colour profiles and force sRGB + 8-bit quantum so
    // channel values survive as plain 0..255 on Q16HDRI builds.
    image.strip();
    image.colorSpace(MagickCore::sRGBColorspace);
    image.depth(8);
    image.type(Magick::TrueColorType);
    return image;
}

/**
 * Scale-down the image so the longer side is at most 256 px, preserving
 * aspect ratio. Matches PIL's Image.thumbnail((256, 256)) used by haishoku.
 */
Magick::Image HaishokuImage::getThumbnail(Magick::Image image) {
    image.thumbnail(Magick::Geometry("256x256"));
    return image;
}

/**
 * Extract all colors from an image as (count, [r, g, b]) tuples using
 * Magick++'s color histogram. Matches PIL's Image.getcolors(w * h) used by
 * haishoku.haillow.get_colors.
 */
std::vector<ColorTuple> HaishokuImage::getColors(std::string &imagePath) {
    HaishokuImage self;
    Magick::Image image = self.getImage(imagePath);
    image = self.getThumbnail(image);

    const size_t width  = image.columns();
    const size_t height = image.rows();
    Magick::Pixels view(image);
    const Magick::Quantum *pixels = view.getConst(0, 0, width, height);
    const size_t channels = image.channels();
    const double qRange = static_cast<double>((Magick::Quantum) 65535.0);

    // Pack (r << 16 | g << 8 | b) into 24-bit key. Clamp to [0, 255] so HDRI
    // quantum overshoot from resampling is treated the same as PIL would.
    std::unordered_map<uint32_t, size_t> counts;
    counts.reserve(width * height);

    auto toByte = [&qRange](double q) -> int {
        double v = q / qRange * 255.0;
        return std::clamp(static_cast<int>(v), 0, 255);
    };

    for (size_t i = 0; i < width * height; ++i) {
        const Magick::Quantum *p = pixels + i * channels;
        int r = toByte(static_cast<double>(p[0]));
        int g = toByte(static_cast<double>(p[1]));
        int b = toByte(static_cast<double>(p[2]));
        uint32_t key = (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
        ++counts[key];
    }

    std::vector<ColorTuple> result;
    result.reserve(counts.size());
    for (const auto &entry : counts) {
        int r = (entry.first >> 16) & 0xff;
        int g = (entry.first >>  8) & 0xff;
        int b =  entry.first        & 0xff;
        result.emplace_back(static_cast<int>(entry.second),
                            std::array<int, 3>{r, g, b});
    }

    if (result.empty()) {
        std::string message = "Empty set of colours extracted from image!";
        throw AppException(message);
    }

    return result;
}
