#include <iostream>

#include "timing.hpp"
#include "common.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "imageio.hpp"

/////////////////////////////////////////////
// GLOBAL VARIABLES

// sphere at -1 of z-axis
point3 initial_sphere_center = point3(0,0,-1);
double initial_sphere_radius = 0.5;


/////////////////////////////////////////////
// RAYTRACING ALGORITHM
double hit_sphere(const point3& center, double radius, const ray& r) {
    // TODO --- TASK 2
    return 0.0;
    // -----TASK-2----
}

color ray_color(const ray& r) {
    // TODO --- TASK 2
    // change ray color to red (rgb) if it hits initial_sphere
    // -----TASK-2----
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
    // TODO --- TASK 4
    // const int samples_per_pixel = 100;
    // -----TASK-4----
    // TODO --- TASK 5
    // const int max_depth = 50;
    // -----TASK-5----
    color* image;
    if((image=(vec3*)malloc(image_width * image_height * sizeof(vec3)))==NULL) {
        fprintf(stderr,"Could not allocate picture memory!\n");
        exit(1);
    }

    // World
    // TODO --- TASK 3
    // hittable_list world;
    //
    // world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    // -----TASK-3----
    // TODO --- TASK 6
    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    // auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.05);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.75);
    //
    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    // -----TASK-6----

    // Camera
    camera cam;

    // Render
    double ws,we,cs,ce;
    timing(&ws,&cs);
    for (int j=0; j<image_height; ++j)
    {
        for (int i=0; i<image_width; ++i)
        {
            // TODO TASK 4: add the samples per pixel loop
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r = cam.get_ray(u, v);
            image[j*image_width + i] = ray_color(r);
        }
    }
    timing(&we,&ce);
    fprintf(stderr,"Time: %lf s, Performance: %lf MPixels/s\n",we-ws,(double)image_height*image_width/(we-ws)/1e6);

    // Write output
    const char* fn = "result.pnm";
    write_pnm(image_width, image_height, image, fn);
}
