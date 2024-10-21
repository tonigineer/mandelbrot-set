#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/Graphics.hpp>

typedef unsigned long long ll;

struct Complex {
    long double real;
    long double imag;
};

class Mandelbrot {
  private:
    const uint32_t screen_width;
    const uint32_t screen_height;

    Complex center_point = {0, 0};

    ll magnification = 1;
    const long double span = 2;

    long double _real_start;
    long double _imag_start;
    long double _delta_real;
    long double _delta_imag;

    // Complex transform_pixel_to_complex(uint16_t pixel_x, uint16_t pixel_y);

  public:
    void _base_calculation(uint32_t x, uint32_t y);

    void _calculate_chunk(uint32_t y_start, uint32_t y_end);

    uint16_t* iterations;
    long double* real_parts;
    long double* imag_parts;

    Mandelbrot(const uint32_t screen_width, const uint32_t screen_height);

    void update();
};

#endif