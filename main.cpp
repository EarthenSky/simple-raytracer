#include <iostream>
#include<unistd.h>

int main() {
    const int image_width = 256;
    const int image_height = 256;

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int y = image_height-1; y >= 0; y--) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush; // \r means go to leftmost part of the line.
        for (int x = 0; x < image_width; x++) {
            auto r = double(x) / (image_width-1); // in [0, 1]
            auto g = double(y) / (image_height-1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }

        usleep(4000); // this helps show off the progress meter.
    }

    std::cerr << "\nDone\n";

    return 0;
}