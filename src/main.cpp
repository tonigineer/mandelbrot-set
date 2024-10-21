#include <SFML/Graphics.hpp>
#include <iostream>

#include "event_handler.hpp"
#include "mandelbrot.hpp"
#include "renderer.hpp"

std::string WINDOW_NAME = "Mandelbrot";

const uint32_t SCREEN_WIDTH = 1280;
const uint32_t SCREEN_HEIGHT = 760;
const uint16_t MAX_FPS = -144;

int main(void) {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_NAME);
    window.setFramerateLimit(MAX_FPS);
    window.setMouseCursorVisible(true);
    window.setKeyRepeatEnabled(false);

    Mandelbrot mandelbrot(SCREEN_WIDTH, SCREEN_HEIGHT);
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
    EventHandler event_handler;

    while (window.isOpen()) {
        event_handler.check(window);
        mandelbrot.update();
        renderer.update(window, &mandelbrot);
    }

    return 0;
}