#include "renderer.hpp"
#include "colors.h"
#include "mandelbrot.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

Renderer::Renderer(const uint32_t screen_width, const uint32_t screen_height)
    : screen_width(screen_width), screen_height(screen_height) {
    if (!font.loadFromFile("./../assets/arial.ttf"))
        std::cerr << "Font not found!" << std::endl;

    // Initialize informative texts
    text_fps.setFont(font);
    text_fps.setCharacterSize(FONT_SIZE);
    text_fps.setFillColor(sf::Color(0xFF, 0xFF, 0x00));

    // Initialize `canvas`
    screen_texture.create(screen_width, screen_height);
    screen_sprite.setTexture(screen_texture);
    pixels = new sf::Uint8[screen_width * screen_height * RGBA_SIZE];
}

void Renderer::update(sf::RenderWindow& window, Mandelbrot* mandelbrot) {
    _update_fps_text();
    _update_screen_pixel(mandelbrot);

    screen_texture.update(pixels);
    screen_sprite.setTexture(screen_texture);

    window.clear();
    window.draw(screen_sprite);
    window.draw(text_fps);
    window.display();
}

void Renderer::_update_screen_pixel(Mandelbrot* mandelbrot) {
    // Source: https://github.com/josch/mandelbrot (Wikipedia animation)
    const long double Q1_LOG_2 = 1.44269504088896340735992468100189213742664595415299;
    const long double LOG_2 = 0.69314718055994530941723212145817656807550013436026;
    const long double BAILOUT = 128.0;
    const long double LOG_LOG_BAILOUT = log(log(BAILOUT));

    size_t n_pixel = 0;

    for (size_t i = 0; i < screen_width * screen_height; i++) {

        long double real_squared = mandelbrot->real_parts[i] * mandelbrot->real_parts[i] +
                                   mandelbrot->imag_parts[i] * mandelbrot->imag_parts[i];

        if (real_squared < BAILOUT) {
            pixels[n_pixel++] = 0;
            pixels[n_pixel++] = 0;
            pixels[n_pixel++] = 0;
            pixels[n_pixel++] = 255;
            continue;
        };

        long double r = sqrtl(real_squared);
        long double c = mandelbrot->iterations[i] - 1.28 + (LOG_LOG_BAILOUT - logl(logl(r))) * Q1_LOG_2;
        size_t idx = fmodl((logl(c / 64 + 1) / LOG_2 + 0.45), 1) * GRADIENT_LENGTH + 0.5;

        pixels[n_pixel++] = COLOR_TABLE[idx][0];
        pixels[n_pixel++] = COLOR_TABLE[idx][1];
        pixels[n_pixel++] = COLOR_TABLE[idx][2];
        pixels[n_pixel++] = 255;

        // // KILLME: Less computational color scheme
        // pixels[n_pixel++] = mandelbrot->iterations[i] % 256;
        // pixels[n_pixel++] = 1 / 255 * mandelbrot->iterations[i] % 256;
        // pixels[n_pixel++] = mandelbrot->iterations[i] * mandelbrot->iterations[i] % 256;
        // pixels[n_pixel++] = 255;
    }
}

void Renderer::_update_fps_text() {
    text_fps.setString("FPS: " + std::to_string(1 / clock.restart().asSeconds()) +
                       "\nFrametime: " + std::to_string(clock.getElapsedTime().asSeconds()));
}
