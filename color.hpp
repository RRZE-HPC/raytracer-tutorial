#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    auto r = sqrt(pixel_color.x());
    auto g = sqrt(pixel_color.y());
    auto b = sqrt(pixel_color.z());

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

void write_color_to_file(FILE* fp, color pixel_color) {
    auto r = sqrt(pixel_color.x());
    auto g = sqrt(pixel_color.y());
    auto b = sqrt(pixel_color.z());

    // Write the translated [0,255] value of each color component.
    fprintf(fp, "%d %d %d\n",
        static_cast<int>(256 * clamp(r, 0.0, 0.999)),
        static_cast<int>(256 * clamp(g, 0.0, 0.999)),
        static_cast<int>(256 * clamp(b, 0.0, 0.999)));
}

#endif
