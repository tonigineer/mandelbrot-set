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

    uint16_t determine_iterations(Complex point);

    Complex transform_pixel_to_complex(uint16_t pixel_x, uint16_t pixel_y);

  public:
    uint16_t* iterations;

    Mandelbrot(const uint32_t screen_width, const uint32_t screen_height);

    void update();
};

#endif