#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>

class Renderer {
  private:
    const uint8_t FONT_SIZE = 32;
    const uint8_t RGBA_SIZE = 4;

    const uint32_t screen_width;
    const uint32_t screen_height;

    sf::Clock clock;
    sf::Font font;

    sf::Text text_fps;
    sf::Uint8* pixels;

    sf::Texture screen_texture;
    sf::Sprite screen_sprite;

    void _update_fps_text();

    void _update_screen_pixel();

  public:
    Renderer(const uint32_t screen_width, const uint32_t screen_height);

    void update(sf::RenderWindow& window);
};

#endif