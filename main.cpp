#include <iostream>
#include <cstring>

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


void calc_tile(int image_width, int image_height, int xstart, int ystart, int tilesize, int samples_per_pixel, color* tile, camera cam, hittable_list world, int max_depth) {
    int i = 0;
    for (int y = ystart; y < ystart+tilesize; y++)
    {
        for (int x = xstart; x < xstart+tilesize; x++)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = double(random_double() + x) / (image_width-1);
                auto v = double(random_double() + y) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            tile[i++] = pixel_color / samples_per_pixel;
        }
    }
}


/////////////////////////////////////////////
// MAIN
int main() {
    // Image
    const auto aspect_ratio = 1; //16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;
    const int tilesize = 100;
    color* image;
    if((image=(vec3*)malloc(image_width * image_height * sizeof(vec3)))==NULL) {
        fprintf(stderr,"Could not allocate picture memory!\n");
        exit(1);
    }
    color* tile;

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

    // number of tiles in x and y direction
    int xtiles = image_width / tilesize;
    int ytiles = image_height / tilesize;

    #pragma omp parallel private(tile, world, cam)
    {
        if((tile=(vec3*)malloc(tilesize*tilesize*sizeof(vec3)))==NULL) {
            fprintf(stderr,"Could not allocate tile memory!\n");
            exit(1);
        }

        #pragma omp for schedule(dynamic) collapse(2)
        for(int yc=0; yc<xtiles; yc++)
        {
            for(int xc=0; xc<ytiles; xc++)
            {
                // calc one tile
                calc_tile(image_width, image_height, xc*tilesize, yc*tilesize, tilesize, samples_per_pixel, tile, cam, world, max_depth);
                // copy to picture buffer
                for(int i=0; i<tilesize; i++)
                {
                    int tilebase = yc*tilesize*tilesize*xtiles+xc*tilesize;
                    memcpy((void*)(image+tilebase+i*tilesize*xtiles),
                           (void*)(tile+i*tilesize),
                           tilesize*sizeof(color));
                }
            }
        }
        free(tile);
    }

    //#pragma omp parallel for schedule(dynamic) collapse(2) firstprivate(world, cam)
    //for (int j=0; j<image_height; ++j)
    //{
    //    for (int i=0; i<image_width; ++i)
    //    {
    //        color pixel_color(0, 0, 0);
    //        for (int s = 0; s < samples_per_pixel; ++s)
    //        {
    //            auto u = double(random_double() + i) / (image_width-1);
    //            auto v = double(random_double() + j) / (image_height-1);
    //            ray r = cam.get_ray(u, v);
    //            pixel_color += ray_color(r, world, max_depth);
    //        }
    //        image[j*image_width + i] = (pixel_color / samples_per_pixel);
    //    }
    //}
    timing(&we,&ce);
    fprintf(stderr,"Time: %lf s, Performance: %lf MPixels/s\n",we-ws,(double)image_height*image_width/(we-ws)/1e6);

    // Write output
    const char* fn = "result.pnm";
    write_pnm(image_width, image_height, image, fn);
}
