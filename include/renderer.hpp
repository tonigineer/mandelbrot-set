#ifndef RENDERER_H
#define RENDERER_H

#include "mandelbrot.hpp"
#include "mandelbrot.hpp"
#include <SFML/Graphics.hpp>

class Renderer {
  private:
    uint32_t screen_width;
    uint32_t screen_height;
    double zoom_factor = 2.0;

    sf::Texture screen_texture;
    sf::Sprite screen_sprite;

    sf::Clock clock;
    sf::Text info_text;
    sf::RectangleShape info_text_box;
    sf::Font font;

    sf::Event event;

    void _key_press_mappings(sf::Event& event, sf::RenderWindow& window, Mandelbrot& mandelbrot);

  public:
    sf::Uint8* pixels;
    sf::RenderWindow window;

    Renderer(const uint32_t screen_width, const uint32_t screen_height);

    void check_events(sf::RenderWindow& window, Mandelbrot& mandelbrot);

    void update(Mandelbrot* mandelbrot);

    void show();
};

#endif