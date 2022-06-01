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
Note that er is a `dot()` helper function for `vec3`. Instead of returning the standard color, return only the color red if the ray hits the sphere. (*Hint: First, calculate $a, b, c$ and then resolve for the discriminant. Remember the discrimant is* $\sqrt{b^2 - 4ac}$)

You now should see something like this:
![result 02](/docs/images/res02.png)
-------------------------------
The tutorial images and texts are taken from [_Scratchapixel's Introduction to ray tracing_](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/) and [Wikipedia](https://de.wikipedia.org/wiki/Raytracing).

All code is based on [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) and [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html).

Authors of this repo are:
- [Jan Laukemann](mailto:jan.laukemann@fau.de)
