### Solution

### main.cpp
```cpp
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return color(0,0,0);
	}

	if (world.hit(r, 0.001, infinity, rec)) {
    	point3 target = rec.p + rec.normal + random_unit_vector();
	    return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
	}

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

// ...
int main() {
	// ...
    // Render
	// for j
        // for i
            // for s
                // u = ...
                // v = ...
                // ray r = ...
                pixel_color += ray_color(r, world, max_depth);
    // ...
}
```
