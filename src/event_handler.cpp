#include "event_handler.hpp"
#include <SFML/Graphics.hpp>

EventHandler::EventHandler() {}

void EventHandler::check(sf::RenderWindow& window) {
    while (window.pollEvent(event)) {
        switch (event.type) {

        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyPressed:
            key_press_mapping(event, window);
            break;

        default:
            break;
        }
    }
}

void EventHandler::key_press_mapping(sf::Event& event, sf::RenderWindow& window) {
    switch (event.key.code) {

    case sf::Keyboard::Escape:
        window.close();
        break;

    default:
        break;
    }
}
