# Raytracer tutorial

In this tutorial you will learn how to program your own ray tracer step by step.
Please follow the instructions closely.

# Task 0 - Theory
<details>
<summary>Learn about the theory of raytracing</summary>
<br>
We can visualize a picture as a cut made through a pyramid whose apex is located at the center of our eye and whose height is parallel to our line of sight:

![Initial idea](/docs/images/rt-algo-01.png)

Now, we can project the four corners of the front face on the canvas:
![Projecting to canvas](/docs/images/rt-algo-02.gif)

Instead of tracing rays from the light source to the receptor (such as our eye), we trace rays backward from the receptor to the objects. Because this direction is the reverse of what happens in nature, it is fittingly called **backward ray-tracing** or **eye tracing** because we shoot rays from the eye position:
![Backward tracing](/docs/images/rt-algo-03.gif)

Ray-tracing is, therefore, elegant in the way that it is based directly on what actually happens around us. Apart from the fact that it follows the path of light in the reverse order, it is nothing less that a perfect nature simulator.

In the ray tracer program, we shoot a primary ray through the center of the pixel to check for a possible object intersection. When we find one we then cast a shadow ray to find out if the point is illuminated or in shadow:
![Shadow ray 1](/docs/images/rt-algo-04.gif)

If this particular ray does not intersect an object on its way to the light, the hit point is illuminated. If it does intersect with another object, that object casts a shadow on it:
![Shadow ray 2](/docs/images/rt-algo-05.gif)

If we repeat this operation for every pixel, we obtain a two-dimensional representation of our three-dimensional scene:
![Rendered frame](/docs/images/rt-algo-06.gif)

By extending the idea of ray propagation, we can very easily simulate effects like **reflection** and **refraction**.
- For the *reflection ray*, we need two items: the normal at the point of intersection and the primary ray's direction. Once we obtain the reflection direction, we shoot a new ray in that direction.
- For the refraction, we compute the **transmission ray** with the normal at the hit point, the primary ray direction, and the refractive index of the material.
  At the other side of the object, because it changes medium, the ray is refracted one more time.

![Reflection and refraction](/docs/images/rt-algo-07.gif)

Note, that this algorithm is **recursive** and rays can bounce of objects endlessly.
For this reason, we have to set an arbitrary limit that prevents the rays from interacting (i.e., recursing) endlessly.

As summary with equations:
![raytracing algorithm](/docs/images/rt-algo-wiki.png)

</details>
<br><br>

# Task 1 - Basic code structure
Get familiar with the code!
Look at
- `vec3.hpp`
- `color.hpp`
- `ray.hpp`
- `camera.hpp`
- `main.cpp`

The camera geometry looks as the following:
![camera geometry](/docs/images/rt-cam-geom.jpg)

Try to compile (`make`) and run (`./raytrc.exe`) the program and look at the result.
```shell
$ make
$ ./raytrc.exe
Time: 0.001072 s, Performance: 83.942041 MPixels/s
$ display result.pnm
```
You should see a blue-to-white gradient depending on ray Y coordinate:
![result 01](/docs/images/res01.png)

# Task 2 - Add a sphere
We now want to add our first sphere in the image.
To calculate if a ray hits, we use simple high school math.
A point $\mathbf{P}=(x,y,z)$ is inside a sphere with center $\mathbf{C}=(C_x,C_y,C_z)$ and radius $r$, if
$$ (\mathbf{P}-\mathbf{C}) \cdot (\mathbf{P}-\mathbf{C}) = r^2 $$
With a ray being a function of points $\mathbf{P}(t) = \mathbf{A} + t\mathbf{b}$ with the the origin $A$ and the direction $b$, there are some $t$ for with $\mathbf{P}(t)$ satisfies this equation.  
Expanding the equation to the full form and move everything to the left hand side we get:

$$ t^2\mathbf{b} \cdot \mathbf{b} + 2t\mathbf{b} \cdot (\mathbf{A} - \mathbf{C}) + (\mathbf{A} - \mathbf{C}) \cdot (\mathbf{A} - \mathbf{C}) - r^2 = 0 $$

This equation system can be solved via the standard solution for quadratic equation of the structure "$ax^2 + bx + c$" and we know the ray hits the sphere if the **discriminant is larger than $0$**.

**Complete the `hit_sphere()` and `ray_color()` functions in `main.cpp` according to the explanation**  
To be ready for further tasks, let's return `-1.0` if the discriminant is lower than `0` (i.e., misses the sphere) and $x$ otherwise.  
Note that there is a `dot()` helper function for `vec3`. Instead of returning the standard color, return only the color red if the ray hits the sphere. (*Hint: First, calculate $a, b, c$ and then resolve for the discriminant. Remember the solution for x is $x = \frac{-b - \sqrt{b^2 - 4ac}}{2a}$, we do not care about the case of adding the root of the discriminant since it would give us the outgoing intersection.*)

You now should see something like this:

![result 02](/docs/images/res02.png)

# Task 3 - Normal-colored sphere and ground
Let us color the sphere according to its normal vectors.
For this, please change your strictly red coloring (probably like `return color(1, 0, 0);`) to these lines in case of a hit in `main.cpp`:
```cpp
// if hit_sphere returns > 0.0:
vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
// else:
// vec3 unit_direction = ...
```

Result:

![result 03](/docs/images/res03.png)

Since there are some serious design decision we have to take, like having an abstraction class for all kind of objects and not just spheres, a differentiation between the front side and the back side of a surface and a direction of the normal depending on the direction of the incoming ray (from the inside vs from the outside), we will refactor our project a bit:
1. From now on we have a abstract class `hittable` and a class `sphere` that is a hittable object. Look at `hittable.hpp` and `sphere.hpp`.  
  **Include `sphere.hpp` in your `main.cpp`!**
2. To handle more than one object, we will have a list of `hittable`s and finally some common constants and functions. Look at `hittable_list.hpp` and `common.hpp`.  
  **Include both header files in your `main.cpp`!**  
3. The function determining if an object was hit is now handled in the corresponding object classes (like `sphere`), therefore, we don't need the `hit_sphere()` function in main anymore.  
  **Delete `hit_sphere()` in `main.cpp`**
4. We now can create our objects in a smarter way! Look at the `// World` section for Task 3 in the main function and comment it in.
5. **Replace the current `ray_color()` function with one including our list of `hittable`s:**  
    ```cpp
    color ray_color(const ray& r, const hittable& world) {
        hit_record rec;
        if (world.hit(r, 0.001, infinity, rec))
        {
            return 0.5 * (rec.normal + color(1,1,1));
        }
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
    }
    ```
    We still do the same as before and simply replaced the specific calculations for a sphere by the generic `world.hit()`.  
    **Adjust the `ray_color()` call in the main() function accordingly!**

Your world should now look like this:

![result 03b](/docs/images/res03b.png)


# Task 4 - Antialiasing
So far we always rendered one sample per pixel. For smoother edges and object boundaries, it is helpful to increase this number! This is called **antialiasing**.
For this, **add the `samples_per_pixel` variable in the image section**.

In our main rendering loop, we now want to create a new loop iterating over the number of samples per pixel.
To vary the position of our rays inside a pixel, we add a random number in the range of 0 to 1 (check out `common.hpp`) to our `i` and `j` coordinate.
Instead of writing our color directly in the image, we first accumulate all of our ray colors into a pixel color, that is initially **black** for each pixel.
Finally, to get the average of all colors, we divide our **final** pixel color by the number of samples per pixel.
Your picture should now show much smoother edges, check it out!


# Task 5 - Diffusion
We now want to generate a random diffuse bounce ray.
When a ray is reflected, it bounces randomly within a tangent sphere based on the normal of the intersection at the surface and has a uniform-like distribution with a higher probability of being scattered close to the normal (you can read about the [Lambertian reflection here](https://en.wikipedia.org/wiki/Lambertian_reflectance)):

![Lambertian reflection](/docs/images/rt-lambertian.png)

Luckily, the randomization is done in the `vec3::random_unit_vector()` function. So far, if a ray hits a surface (`if (world.hit(...)) == true`), we do this:
```cpp
return 0.5 * (rec.normal + color(1,1,1));
```
In case of a hit, let us first create a point `target` ($S$ in our scratch) which position is the sum of the surface intersection (point of our record, $P$ in the scratch), this point's normal and the random unit vector.
Just like in theory and multiplied with the same constant attenuation of `0.5`, we now *recursively* start another `ray_color` simulation, but with our new ray as input (having $P$ as center and the vector $PS$ as direction).

To not end up in a endless loop, we need some kind of break condition. This will be out `max_depth` variable (commented in Image section). Add it as parameter in your `ray_color()` function, decrease it by one every time you recursively call the function and implement a break condition if we reached the max depth. In this case we just return **black**.

Now, the image looks like this (and take much longer to render):

![result 05](/docs/images/res05.png)


# Task 6 - Shiny and fuzzy materials
After adding diffusion, we introduce different materials.
Look at the classes in `material.hpp`. All of theses do basically two things:
- Produce a scattered ray (or say it absorbed the incident ray)
- If scattered, say how much the ray should be attenuated.

For including the material in our objects, we need to adjust several other classes:
- a) Add a shared material pointer (`shared_ptr<material> mat_ptr`) in the `hit_record` struct so the ray can interact with the surface. b) Add the a member of the same nature in the `sphere` class. Since a sphere has the same material from the beginning, we need to adjust the (non-empty) constructor: `sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};`. c) Finally, in the `hit()` function, of a sphere, we assing the sphere's material to the given record.
- In `main.cpp`, we need to adjust our `ray_color()` function. The reflected ray is now not simply reflected with the same color and strength, but based on the `scatter()` function of the material. Inside our hit-branch, we declare a ray `scattered` and a color `attenuation` and pass those together with the according parameters to the mentioned `scatter()`. This either assigns a value to the newly created variables (and returns `true`) or we simply return *black*. In the former case, we can now use the attenuation instead of the previous constant value (`0.5`) and pass the newly calculated ray (*instead of the now obsolete `target`*) for the recursive call.

We can now remove the two spheres added to the world in _Task 3_ and instead include the _Task 6_ section, adding four spheres (ground, center, left, right) of different materials to the scene (don't forget to also include the header file):

![result 06](/docs/images/res06.png)

# Task 7 - Parallelization
By now, the rendering already takes a lot of time.
Let's parallelize it!

-------------------------------
The tutorial images and texts are taken from [_Scratchapixel's Introduction to ray tracing_](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/) and [Wikipedia](https://de.wikipedia.org/wiki/Raytracing).

Most of the code and tasks are based on [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) and [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html).

Authors of this repo are:
- [Jan Laukemann](mailto:jan.laukemann@fau.de)
