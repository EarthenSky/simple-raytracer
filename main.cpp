#include "rtweekend_util.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include <unistd.h>

// the main "draw" function
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
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
    const int samples_per_pixel = 100;

    // World

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5)); // target sphere
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); // ground

    // Camera

    camera cam;

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // TODO: check how much performance we lose if we do rays in random order.

    for (int y = image_height-1; y >= 0; y--) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush; // \r means go to leftmost part of the line.

        for (int x = 0; x < image_width; x++) {
            
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (x + random_double()) / (image_width-1);
                auto v = (y + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world); // A real example of polymorphism!
            }
            write_color(std::cout, pixel_color, samples_per_pixel);

        }

        //usleep(100); // this helps show off the progress meter.
    }

    std::cerr << "\nDone\n";

    return 0;
}