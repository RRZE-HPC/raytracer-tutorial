#include <iostream>

#include "timing.hpp"
#include "common.hpp"
#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "imageio.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

/////////////////////////////////////////////
// GLOBAL VARIABLES

// sphere at -1 of z-axis
point3 initial_sphere_center = point3(0,0,-1);
double initial_sphere_radius = 0.5;


/////////////////////////////////////////////
// RAYTRACING ALGORITHM
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


/////////////////////////////////////////////
// MAIN
int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    // TODO --- TASK 5
    // const int max_depth = 50;
    // -----TASK-5----
    color* image;
    if((image=(vec3*)malloc(image_width * image_height * sizeof(vec3)))==NULL) {
        fprintf(stderr,"Could not allocate picture memory!\n");
        exit(1);
    }

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;

    // Render
    double ws,we,cs,ce;
    timing(&ws,&cs);
    for (int j=0; j<image_height; ++j)
    {
        for (int i=0; i<image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = double(random_double() + i) / (image_width-1);
                auto v = double(random_double() + j) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            image[j*image_width + i] = (pixel_color / samples_per_pixel);
        }
    }
    timing(&we,&ce);
    fprintf(stderr,"Time: %lf s, Performance: %lf MPixels/s\n",we-ws,(double)image_height*image_width/(we-ws)/1e6);

    // Write output
    const char* fn = "result.pnm";
    write_pnm(image_width, image_height, image, fn);
}
