#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <unistd.h>

// this is just a white-blue gradient -> a lerp
color ray_color(const ray& r) {
    // components are bounded between [-1, 1] -> perfect for a colour!
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.25 * (unit_direction.y() + unit_direction.x() + 2.0); // bounded by [0, 1] now
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.4, 0.6, 1);
}

int main() {

    // Image
    
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length); // -z is viewport direction

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int y = image_height-1; y >= 0; y--) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush; // \r means go to leftmost part of the line.

        for (int x = 0; x < image_width; x++) {
            auto u = double(x) / (image_width-1);
            auto v = double(y) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }

        usleep(1000); // this helps show off the progress meter.
    }

    std::cerr << "\nDone\n";

    return 0;
}