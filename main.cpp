#include <iostream>

#include "timing.hpp"
#include "common.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "imageio.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "material.hpp"

/////////////////////////////////////////////
// GLOBAL VARIABLES

// sphere at -1 of z-axis
point3 initial_sphere_center = point3(0,0,-1);
double initial_sphere_radius = 0.5;


/////////////////////////////////////////////
// RAYTRACING ALGORITHM
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return color(0,0,0);
    }

    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color(0,0,0);
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
    const int max_depth = 50;
    color* image;
    if((image=(vec3*)malloc(image_width * image_height * sizeof(vec3)))==NULL) {
        fprintf(stderr,"Could not allocate picture memory!\n");
        exit(1);
    }

    // World
    hittable_list world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.05);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.75);
    
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

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
                pixel_color += ray_color(r, world, max_depth);
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
