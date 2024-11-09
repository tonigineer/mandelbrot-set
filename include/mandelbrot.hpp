#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/Graphics.hpp>

// std::complex not needed for such simple calculations.
// TODO Maybe faster?
struct Complex {
    long double real;
    long double imag;
};

const long double PRESETS[22][2] = {
    {+0.382308869L, -0.3895870170L},
    {+0.288935044L, -0.0123748250L},
    {+0.286016756L, +0.0115598130L},
    {+0.276643312L, +0.0091976760L},
    {+0.235336955L, -0.5152623050L},
    {+0.115116634L, +0.6278655140L},
    {+0.438204695L, +0.3420529940L},
    {-0.028375015L, +0.6946544980L},
    {-0.106750445L, +0.8828258480L},
    {-0.673281229L, +0.3600723860L},
    {-0.743643887037158704752191506114774L, +0.131825904205311970493132056385139L},
    {-1.009078747L, +0.3108562000L},
    {-1.167468173L, -0.2461746550L},
    {-1.167468173L, -0.2461746550L},
    {-1.168235004L, -0.2462116668L},
    {-1.168235004L, -0.2462116668L},
    {-1.394420575L, -0.0018272120L},
    {-1.394420575L, -0.0018272120L},
    {-1.403445777L, +0.0000000120L},
    {-1.768778829L, +0.0017389240L},
    {-1.940157353L, +0.0000000000L},
};

class Mandelbrot {
  public:
    const uint32_t width;
    const uint32_t height;

    long double zoom_factor = 2L;
    uint32_t n_iter_max = 128U;

    int region_index = 0;
    Complex center_point = {PRESETS[region_index][0], PRESETS[region_index][1]};

    long double magnification = 1.0L;

  private:
    long double _real_start;
    long double _imag_start;
    long double _delta_real;
    long double _delta_imag;

  public:
    bool has_changed = true;
    double frame_time;

    long double* real_parts;
    long double* imag_parts;
    uint32_t* iterations;

  public:
    Mandelbrot(const uint32_t width, const uint32_t height);

    void update();

    void _base_algorithm(uint32_t x, uint32_t y);

    void _calculate_chunk(uint32_t y_start, uint32_t y_end);

    void change_region(const int increment);
};

#endif