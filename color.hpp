#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * r) << ' '
        << static_cast<int>(255.999 * g) << ' '
        << static_cast<int>(255.999 * b) << '\n';
}

void write_color_to_file(FILE* fp, color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Write the translated [0,255] value of each color component.
    fprintf(fp, "%d %d %d\n",
        static_cast<int>(255.999 * r),
        static_cast<int>(255.999 * g),
        static_cast<int>(255.999 * b));
}

void write_color_to_file(FILE* fp, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

	// Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    fprintf(fp, "%d %d %d\n",
        static_cast<int>(255.999 * pixel_color.x()),
        static_cast<int>(255.999 * pixel_color.y()),
        static_cast<int>(255.999 * pixel_color.z()));
}

#endif
