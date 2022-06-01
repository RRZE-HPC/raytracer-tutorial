#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "vec3.hpp"
#include "color.hpp"

inline void write_pnm(size_t w, size_t h, const vec3* img, const char* fname) {
    FILE* fp;

    fp = fopen(fname, "w");
	if (fp == NULL) {
		fprintf(stderr,"Could not open output file!\n");
		exit(1);
	}
    fprintf(fp, "P3\n%d %d\n%d\n", (int)w, (int)h, 255);

    for (size_t i = 0; i < w * h; ++i) {
		write_color_to_file(fp, img[i]);
    }

    fclose(fp);
}
#endif
