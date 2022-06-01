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
$$ 
t^2\mathbf{b} \cdot \mathbf{b} + 2t\mathbf{b} \cdot (\mathbf{A} - \mathbf{C}) + (\mathbf{A} - \mathbf{C}) \cdot (\mathbf{A} - \mathbf{C}) - r^2 = 0
$$
This equation system can be solved via the standard solution for quadratic equation of the structure "$ax^2 + bx + c$" and we know the ray hits the sphere if the **discriminant is larger than $0$**.

**Complete the `hit_sphere()` and `ray_color()` functions in `main.cpp` according to the explanation**  
To be ready for further tasks, let's return `-1.0` if the discriminant is lower than `0` (i.e., misses the sphere) and $x$ otherwise.  
Note that there is a `dot()` helper function for `vec3`. Instead of returning the standard color, return only the color red if the ray hits the sphere. (*Hint: First, calculate $a, b, c$ and then resolve for the discriminant. Remember the solution for x is* $x = \frac{-b - \sqrt{b^2 - 4ac}}{2a}$)

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
  Since some of the current `include`s are already covered in the newly added headers, you can remove `ray.hpp` and `vec3.hpp` again.
3. The function determining if an object was hit is now handled in the corresponding object classes (like `sphere`), therefore, we don't need the `hit_sphere()` function in main anymore.  
  **Delete `hit_sphere()` in `main.cpp`**
4. We now can create our objects in a smarter way! Look at the `// World` section for Task 3 in the main function and comment it in.
5. **Replace the current `ray_color()` function with one including our list of `hittable`s:**  
    ```cpp
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
    ```
    We still do the same as before and simply replaced the specific calculations for a sphere by the generic `world.hit()`.  
    **Adjust the `ray_color()` call in the main() function accordingly!**

-------------------------------
The tutorial images and texts are taken from [_Scratchapixel's Introduction to ray tracing_](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/) and [Wikipedia](https://de.wikipedia.org/wiki/Raytracing).

All code is based on [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) and [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html).

Authors of this repo are:
- [Jan Laukemann](mailto:jan.laukemann@fau.de)
