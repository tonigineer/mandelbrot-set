#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/Graphics.hpp>

class Mandelbrot {
  private:
    const uint32_t screen_width;
    const uint32_t screen_height;

    long double real_center = 0;
    long double imag_center = 0;

    unsigned long long magnification = 1;
    const long double span = 2;

  public:
    uint16_t* iterations;

    Mandelbrot(const uint32_t screen_width, const uint32_t screen_height);

    void update();
};

#endif