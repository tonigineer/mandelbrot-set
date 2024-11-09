#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#include "mandelbrot.hpp"
#include "renderer.hpp"

void interactive_mode(const uint16_t screen_width, const uint16_t screen_height) {
    std::cout << "[INFO] Interactive mode started ... \n";

    Mandelbrot mandelbrot(screen_width, screen_height);
    Renderer renderer(mandelbrot.width, mandelbrot.height);

    while (renderer.window.isOpen()) {
        renderer.check_events(renderer.window, mandelbrot);
        mandelbrot.update();
        renderer.update(&mandelbrot);
        renderer.show();
    }

    std::cout << "[INFO] Interactive mode terminated." << std::endl;
}

void export_frame(char* argv[]) {
    Mandelbrot mandelbrot(std::stoi(argv[1]), std::stoi(argv[2]));

    mandelbrot.center_point.real = std::stold(argv[3]);
    mandelbrot.center_point.imag = std::stold(argv[4]);
    mandelbrot.n_iter_max = std::stoi(argv[5]);
    mandelbrot.magnification = std::stold(argv[6]);

    // Calculate Mandelbrot and pixel colors
    // TODO: Make color function as a static function to get rid of Renderer instance
    Renderer renderer(mandelbrot.width, mandelbrot.height);
    mandelbrot.update();
    renderer.update(&mandelbrot);

    // Output as ppm format to stdout, to be further process by scripts.
    std::ostringstream oss;

    oss << "P3\n" << mandelbrot.width << "\n" << mandelbrot.height << "\n255\n";
    for (size_t i = 0; i < mandelbrot.width * mandelbrot.height; i++) {
        oss << (int)renderer.pixels[4 * i + 0] << " " << (int)renderer.pixels[4 * i + 1] << " "
            << (int)renderer.pixels[4 * i + 2] << "\n";
    }
    std::cout << oss.str();
};

int main(int argc, char* argv[]) {
    switch (argc) {
    case 1:
        interactive_mode(1280, 720);
        break;
    case 3:
        interactive_mode(std::stoi(argv[1]), std::stoi(argv[2]));
        break;
    case 7:
        export_frame(argv);
        break;
    }
    return 0;
}
