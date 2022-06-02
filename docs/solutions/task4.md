### Solution

### main.cpp
```cpp
// ...
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
```
