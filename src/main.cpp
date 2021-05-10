#include "rtweekend_util.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <iostream>
#include <unistd.h>

// the main "draw" function
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;
    
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);
    
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    // components are bounded between [-1, 1] -> perfect for a colour!
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.25 * (unit_direction.y() + unit_direction.x() + 2.0); // bounded by [0, 1] now
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.4, 0.6, 1);
}

int main() {

    // Image
    
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 200;
    const int max_depth = 50;


    // World

    hittable_list world;
    auto material_center = make_shared<lambertian>(color(1.0, 0.3, 0.3));
    auto material_ground = make_shared<lambertian>(color(0.3, 0.8, 0.4));
    auto material_left   = make_shared<metal>(color(0.2, 0.4, 1.0), 0.025);
    auto material_right  = make_shared<metal>(color(0.9, 0.9, 0.9), 1.0);

    world.add(make_shared<sphere>(point3(0, 0, -1.6), 0.5, material_center)); // target sphere
    world.add(make_shared<sphere>(point3(0, -100.5, -1.0), 100, material_ground)); // ground
    world.add(make_shared<sphere>(point3(-0.75, 0, -1.15), 0.35, material_left));
    world.add(make_shared<sphere>(point3( 0.75, 0, -1.15), 0.35, material_right));


    // Camera

    camera cam;

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // TODO: check how much performance we lose if we do rays in random order -> what if gpu accelerated?

    for (int y = image_height-1; y >= 0; y--) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush; // \r means go to leftmost part of the line.

        for (int x = 0; x < image_width; x++) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (x + random_double()) / (image_width-1);
                auto v = (y + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth); // A real example of polymorphism!
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone\n";
    return 0;
}