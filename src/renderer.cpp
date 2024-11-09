#include "renderer.hpp"
#include "colors.h"
#include "font_data.h"
#include "mandelbrot.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string WINDOW_NAME = "Mandelbrot-Set Visualizer";
const uint8_t RGBA_SIZE = 4; // RGA Alpha color codes have four values

std::string toScientificString(double value, int precision) {
    std::ostringstream out;
    out << std::scientific << std::setprecision(precision) << value;
    return out.str();
}

std::string toStringWithPrecision(double value, int precision) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

std::string longDoubleToString(long double value) {
    std::ostringstream out;
    out << std::setprecision(std::numeric_limits<long double>::digits10) << value;
    return out.str();
}

Renderer::Renderer(const uint32_t screen_width, const uint32_t screen_height)
    : screen_width(screen_width), screen_height(screen_height) {
    // General window
    window.create(sf::VideoMode(screen_width, screen_height), WINDOW_NAME, sf::Style::None);
    window.setFramerateLimit(72);
    window.setMouseCursorVisible(true);
    window.setKeyRepeatEnabled(false);

    // Kind of the pixel canvas
    screen_texture.create(screen_width, screen_height);
    screen_sprite.setTexture(screen_texture);
    pixels = new sf::Uint8[screen_width * screen_height * RGBA_SIZE];

    // Info text
    // NOTE: `xxd -i font.ttf > font_data.h`
    font.loadFromMemory(TerminusTTF_ttf, TerminusTTF_ttf_len);

    // Initialize informative texts
    info_text.setFont(font);
    info_text.setCharacterSize(screen_height / 35);
    info_text.setFillColor(sf::Color(0xFF, 0xFF, 0xFF));
    info_text.setPosition(sf::Vector2f(0, screen_height * 0.955));

    info_text_box.setSize(sf::Vector2f(screen_width, screen_height * 0.05));
    info_text_box.setPosition(0.0f, screen_height * 0.95);
    info_text_box.setFillColor(sf::Color::Black);
}

void Renderer::update(Mandelbrot* mandelbrot) {
    // Source: https://github.com/josch/mandelbrot (Wikipedia animation)
    const long double Q1_LOG_2 = 1.44269504088896340735992468100189213742664595415299L;
    const long double LOG_2 = 0.69314718055994530941723212145817656807550013436026L;
    const long double BAILOUT = 128.0L;
    const long double LOG_LOG_BAILOUT = log(log(BAILOUT));

    uint32_t n_pixel = 0;

    for (uint32_t i = 0; i < screen_width * screen_height; i++) {
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

        // KILL ME: Less computational color scheme
        // pixels[n_pixel++] = mandelbrot->iterations[i] % 256;
        // pixels[n_pixel++] = 1 / 255 * mandelbrot->iterations[i] % 256;
        // pixels[n_pixel++] = mandelbrot->iterations[i] * mandelbrot->iterations[i] % 256;
        // pixels[n_pixel++] = 255;
    }

    screen_texture.update(pixels);
    screen_sprite.setTexture(screen_texture);

    // Update info text
    info_text.setString("   [" + std::to_string(mandelbrot->region_index + 1) +
                        "/22]   Real:" + longDoubleToString(mandelbrot->center_point.real) +
                        "   Imag: " + longDoubleToString(mandelbrot->center_point.imag) +
                        "   Magnif.: " + toScientificString(mandelbrot->magnification, 2) +
                        "   MaxIter: " + toStringWithPrecision(mandelbrot->n_iter_max, 0) + "   Zoom-F.: x" +
                        toStringWithPrecision(zoom_factor, 2));
}

void Renderer::show() {
    window.clear();

    window.draw(screen_sprite);
    window.draw(info_text_box);
    window.draw(info_text);

    window.display();
}

void Renderer::check_events(sf::RenderWindow& window, Mandelbrot& mandelbrot) {
    while (window.pollEvent(event)) {
        switch (event.type) {

        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            _key_press_mappings(event, window, mandelbrot);
            break;

        default:
            break;
        }
    }
}

void Renderer::_key_press_mappings(sf::Event& event, sf::RenderWindow& window, Mandelbrot& mandelbrot) {
    switch (event.key.code) {

    case sf::Keyboard::Escape:
        window.close();
        return;

    // Zooming
    case sf::Keyboard::W:
        mandelbrot.magnification *= zoom_factor;
        break;

    case sf::Keyboard::S:
        mandelbrot.magnification /= zoom_factor;
        break;

    case sf::Keyboard::Z:
        if (zoom_factor > 1.25)
            zoom_factor -= 0.25;
        return;

    case sf::Keyboard::X:
        if (zoom_factor < 10.00)
            zoom_factor += 0.25;
        return;

    case sf::Keyboard::R:
        mandelbrot.magnification = 1.0L;
        mandelbrot.n_iter_max = 16U;
        break;

    // Goto over preset locations
    case sf::Keyboard::Q:
        mandelbrot.change_region(-1);
        break;

    case sf::Keyboard::E:
        mandelbrot.change_region(1);
        break;

    // Increase max. iteration steps. High magnification needs lots of iterations!
    case sf::Keyboard::A:
        mandelbrot.n_iter_max = mandelbrot.n_iter_max >> 1 < 16U ? 16U : mandelbrot.n_iter_max >> 1;
        break;

    case sf::Keyboard::D:
        mandelbrot.n_iter_max = mandelbrot.n_iter_max << 1 > 32768U ? 32768U : mandelbrot.n_iter_max << 1;
        break;

    // Shift center with VIM keys
    case sf::Keyboard::H:
        mandelbrot.center_point.real -= (0.1L / mandelbrot.magnification);
        break;

    case sf::Keyboard::J:
        mandelbrot.center_point.imag -= (0.1L / mandelbrot.magnification);
        break;

    case sf::Keyboard::K:
        mandelbrot.center_point.imag += (0.1L / mandelbrot.magnification);
        break;

    case sf::Keyboard::L:
        mandelbrot.center_point.real += (0.1L / mandelbrot.magnification);
        break;

    default:
        return;
    }

    mandelbrot.has_changed = true;

    info_text.setString("   processing ...");
    show();
}
