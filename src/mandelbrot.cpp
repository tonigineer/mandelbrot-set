#include "mandelbrot.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <future>
#include <iostream>

Mandelbrot::Mandelbrot(const uint32_t screen_width, const uint32_t screen_height)
    : screen_width(screen_width), screen_height(screen_height) {

    iterations = new uint16_t[screen_width * screen_height];
    real_parts = new long double[screen_width * screen_height];
    imag_parts = new long double[screen_width * screen_height];
};

void Mandelbrot::update() {
    // Determine the real (x) and imag(y) values based on the center coordinate
    // and magnification. The delta values are used to iterate over all pixel and
    // simply add the delta.
    const long double ld_magnification = (long double)magnification;
    _real_start = -2.0 / ld_magnification + center_point.real;
    _imag_start = 2.0 / ld_magnification * screen_height / screen_width + center_point.imag;
    _delta_real = 4.0 / ld_magnification / screen_width;
    _delta_imag = -4.0 / ld_magnification / screen_width;

    // std::cout << _real_start << " " << _imag_start << " " << _delta_real << " " << _delta_imag
    //           << "\n";

    // long n_iter_max = screen_width * sqrt(magnification);

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 2;

    // std::cout << num_threads << "\n";
    // num_threads = 128;

    std::vector<std::future<void>> futures;
    uint32_t rows_per_thread = screen_height / num_threads;

    for (unsigned int i = 0; i < num_threads; i++) {
        uint32_t start_y = i * rows_per_thread;
        uint32_t end_y = (i == num_threads - 1) ? screen_height : (i + 1) * rows_per_thread;

        // Use async for each chunk
        futures.push_back(std::async(std::launch::async, &Mandelbrot::_calculate_chunk, this, start_y, end_y));
    }

    // Wait for all tasks to complete
    for (auto& fut : futures) {
        fut.get();
    }

    // for (uint32_t y = 0; y < screen_height; y++) {
    //     for (uint32_t x = 0; x < screen_width; x++) {
    //         // _base_calculation(x, y);
    //         std::future<void> result = std::async(std::launch::async, &Mandelbrot::_base_calculation, this, x, y);
    //     }
    // }
}

void Mandelbrot::_calculate_chunk(uint32_t y_start, uint32_t y_end) {
    for (uint32_t y = y_start; y < y_end; y++) {
        for (uint32_t x = 0; x < screen_width; x++) {
            _base_calculation(x, y);
        }
    }
}

void Mandelbrot::_base_calculation(uint32_t x, uint32_t y) {
    long double c_real, c_imag, z_real, z_imag, t_real, t_imag;

    uint16_t n_iter = 0;
    z_real = 0;
    z_imag = 0;

    c_imag = _imag_start + _delta_imag * y;
    c_real = _real_start + _delta_real * x;

    while (n_iter < 800) {
        t_real = z_real * z_real - z_imag * z_imag + c_real;
        t_imag = z_real * z_imag * 2.0 + c_imag;
        z_real = t_real;
        z_imag = t_imag;
        n_iter++;

        if ((z_real * z_real + z_imag * z_imag) >= 128.0)
            break;
    }

    iterations[y * screen_width + x] = n_iter;
    real_parts[y * screen_width + x] = z_real;
    imag_parts[y * screen_width + x] = z_imag;
}

// Complex Mandelbrot::transform_pixel_to_complex(uint16_t pixel_x, uint16_t pixel_y) {}
// #include <iostream>
// #include <SFML/Graphics.hpp>
// #include <cmath>
// #include <limits>
// #include <thread>

// #include "mandelbrot.h"
// #include "colors.h"

// void calcMandelbrotSetPart(
//     sf::Uint8 *pixelColors, Window window, struct Center center, const long double
//     magnification, int part, int numParts)
// {
//     // Pixel are incremented from left for x axis and from top for y axis
//     // Magic numbers correspond to an initial area of x -2 to 2 and y -2 to 2
//     long double x0 = -2 / magnification + center.x;
//     long double y0 = 2 / magnification * window.height / window.width + center.y;
//     long double dx = 4 / magnification / window.width;
//     long double dy = -4 / magnification / window.widt    long double _delta_real;h;

//     long maxIterations = window.width * sqrt(magnification);

//     long double cr, zr, re, ci, zi, im;
//     long double lr, li; // last result to check periodicity

//     long n;
//     int pixel = 0;

//     // Settings for color scheme, source further down below
//     long double Q1_LOG_2 = 1.44269504088896340735992468100189213742664595415299;
//     long double LOG_2 = 0.69314718055994530941723212145817656807550013436026;
//     long double BAILOUT = 128.0; // instead of 4.0 as in definition, caused by color scheme
//     long double LOG_LOG_BAILOUT = log(log(BAILOUT));

//     // Adjust loop for threading
//     int ys = window.height * (part - 1) / numParts;
//     int ye = window.height * (part) / numParts;
//     pixel = window.height * window.width * (part - 1) / numParts * 4;

//     for (int y = ys; y < ye; y++)
//     {
//         ci = y0 + dy * y;
//         for (int x = 0; x < window.width; x++)
//         {
//             zr = 0;
//             zi = 0;
//             cr = x0 + dx * x;

//             n = 0;
//             bool insideSet = true;

//             int checkIteration = 3;
//             int updateRateCheckIter = 10;

//             while (n < maxIterations)
//             {
//                 re = zr * zr - zi * zi + cr;
//                 im = zr * zi * 2.0 + ci;
//                 zr = re;
//                 zi = im;
//                 n++;

//                 if ((zr * zr + zi * zi) >= BAILOUT)
//                 {
//                     insideSet = false;
//                     break;
//                 }

//                 if (fabsl(zr - lr) < std::numeric_limits<long double>::epsilon() &&
//                     fabsl(zi - li) < std::numeric_limits<long double>::epsilon())
//                 {
//                     break;
//                 }

//                 // Keep last value `active` for increasing periods
//                 // so that a period might be detected. Especially
//                 // needed for high magnifications where max iterations
//                 // literally explodes.
//                 // Source: https://github.com/josch/mandelbrot
//                 if ((n & checkIteration) == 0)
//                 {
//                     if (--updateRateCheckIter == 0)
//                     {
//                         updateRateCheckIter = 10;
//                         checkIteration <<= 1;
//                         checkIteration++;
//                     }
//                     lr = zr;
//                     li = zi;
//                 }
//             }

//             if (insideSet)
//             {
//                 pixelColors[pixel++] = 0;
//                 pixelColors[pixel++] = 0;
//                 pixelColors[pixel++] = 0;
//                 pixelColors[pixel++] = 255; // full alpha in RGBA
//             }
//             else
//             {
//                 // Source: https://github.com/josch/mandelbrot (Wikipedia animation)
//                 long double r = sqrtl(zr * zr + zi * zi);
//                 long double c = n - 1.28 + (LOG_LOG_BAILOUT - logl(logl(r))) * Q1_LOG_2;
//                 int idx = fmodl((logl(c / 64 + 1) / LOG_2 + 0.45), 1) * GRADIENT_LENGTH +
//                 0.5;

//                 pixelColors[pixel++] = colors[idx][0];
//                 pixelColors[pixel++] = colors[idx][1];
//                 pixelColors[pixel++] = colors[idx][2];
//                 pixelColors[pixel++] = 255; // full alpha in RGBA
//             }
//         }
//     }
// }

// sf::Uint8 *calcMandelbrotSet(struct Window window, struct Center center, const long double
// magnification)
// {
//     int numThreads = 72;
//     std::thread t[numThreads];

//     sf::Uint8 *pixelColors = new sf::Uint8[window.width * window.height * 4];

//     for (size_t i = 0; i < numThreads; i++)
//     {
//         t[i] = std::thread(calcMandelbrotSetPart,
//                            pixelColors, window, center, magnification, i + 1, numThreads);
//     }

//     for (size_t i = 0; i < numThreads; i++)
//         t[i].join();

//     return pixelColors;
// }