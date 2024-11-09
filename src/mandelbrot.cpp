#include "mandelbrot.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <future>
#include <iostream>

Mandelbrot::Mandelbrot(const uint32_t width, const uint32_t height) : width(width), height(height) {
    iterations = new uint32_t[width * height];
    real_parts = new long double[width * height];
    imag_parts = new long double[width * height];
};

void Mandelbrot::update() {
    if (!has_changed)
        return;

    // Determine the real (x) and imag(y) values based on the center coordinate
    // and magnification. The delta values are used to iterate over all pixel and
    // simply add the delta.
    _real_start = -2.0 / magnification + center_point.real;
    _imag_start = 2.0 / magnification * height / width + center_point.imag;
    _delta_real = 4.0 / magnification / width;
    _delta_imag = -4.0 / magnification / width;

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 2;

    std::vector<std::future<void>> futures;
    uint32_t rows_per_thread = height / num_threads;

    for (size_t i = 0; i < num_threads; i++) {
        uint32_t start_y = i * rows_per_thread;
        uint32_t end_y = (i == num_threads - 1) ? height : (i + 1) * rows_per_thread;

        futures.push_back(std::async(std::launch::async, &Mandelbrot::_calculate_chunk, this, start_y, end_y));
    }

    has_changed = false;
}

void Mandelbrot::_calculate_chunk(uint32_t y_start, uint32_t y_end) {
    for (uint32_t y = y_start; y < y_end; y++) {
        for (uint32_t x = 0; x < width; x++) {
            _base_algorithm(x, y);
        }
    }
}

void Mandelbrot::_base_algorithm(uint32_t x, uint32_t y) {
    long double c_real, c_imag, z_real, z_imag, t_real, t_imag;

    uint16_t n_iter = 0;
    z_real = 0;
    z_imag = 0;

    c_imag = _imag_start + _delta_imag * y;
    c_real = _real_start + _delta_real * x;
    c_imag = _imag_start + _delta_imag * y;
    c_real = _real_start + _delta_real * x;

    while (n_iter < n_iter_max) {
        t_real = z_real * z_real - z_imag * z_imag + c_real;
        t_imag = z_real * z_imag * 2.0 + c_imag;
        z_real = t_real;
        z_imag = t_imag;
        n_iter++;

        if ((z_real * z_real + z_imag * z_imag) >= 128.0)
            break;
    }

    iterations[y * width + x] = n_iter;
    real_parts[y * width + x] = z_real;
    imag_parts[y * width + x] = z_imag;
}

void Mandelbrot::change_region(const int increment) {
    constexpr int num_regions = sizeof(PRESETS) / sizeof(PRESETS[0]);

    region_index = ((region_index + increment) % num_regions + num_regions) % num_regions;

    center_point.real = PRESETS[region_index][0];
    center_point.imag = PRESETS[region_index][1];
    magnification = 1.0L;
}
