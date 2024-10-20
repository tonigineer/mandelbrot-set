#include "renderer.hpp"
#include "mandelbrot.hpp"
#include <SFML/Graphics.hpp>
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

void Renderer::update(sf::RenderWindow& window) {
    _update_fps_text();
    _update_screen_pixel();

    screen_texture.update(pixels);
    screen_sprite.setTexture(screen_texture);

    window.clear();
    window.draw(screen_sprite);
    window.draw(text_fps);
    window.display();
}

void Renderer::_update_screen_pixel(uint16_t& iterations) {
    size_t n_pixel = 0;
    for (uint32_t i = 0; i < screen_width * screen_height; i++) {
        pixels[n_pixel++] = iterations[&i];
        pixels[n_pixel++] = 0;
        pixels[n_pixel++] = 0;
        pixels[n_pixel++] = 255;
    }
}

void Renderer::_update_fps_text() {
    text_fps.setString("FPS: " + std::to_string(1 / clock.restart().asSeconds()));
}
