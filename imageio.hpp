#ifndef IMAGEIO_H
#define IMAGEIO_H

#include "vec3.hpp"
#include "color.hpp"

inline void write_pnm(int w, int h, const vec3* img, const char* fname) {
    FILE* fp;

    fp = fopen(fname, "w");
	if (fp == NULL) {
		fprintf(stderr,"Could not open output file!\n");
		exit(1);
	}
    fprintf(fp, "P3\n%d %d\n%d\n", w, h, 255);

    for (int j=h-1; j>=0; j--) {
        for (int i=0; i<w; ++i) {
    		write_color_to_file(fp, img[j*w+i]);
        }
    }

    fclose(fp);
}
#endif
