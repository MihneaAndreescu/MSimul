#include "TableSingleton.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <random>

int main()
{
    TableSingleton& tableSingletonInstance = TableSingleton::getInstance();

    sf::RenderWindow window(sf::VideoMode(900, 900), "MSimul");
    sf::View view;
    view.setSize(sf::Vector2f(tableSingletonInstance.getSize(), -tableSingletonInstance.getSize()));
    view.setCenter(sf::Vector2f(tableSingletonInstance.getSize(), tableSingletonInstance.getSize()) * 0.5f);
    
    view.setSize(sf::Vector2f(1, -1));
    view.setCenter(sf::Vector2f(0.5, 0.5));

    window.setView(view);

    sf::Clock fpsClock;
    sf::Clock frameClock;
    int fps = 0;
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    float radius = 0.5 * 2;

    while (window.isOpen())
    {
        sf::Clock glock;
        sf::Event event;

        sf::Vector2f mousePositionOrig = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
        sf::Vector2f mousePosition = mousePositionOrig * (float)tableSingletonInstance.getSize();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    radius *= pow(1.1, event.mouseWheelScroll.delta);
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
            {
                std::cout << "enter filename to save: " << std::endl;
                std::string filename;
                std::cin >> filename;
                tableSingletonInstance.saveToFile(filename);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
            {
                std::cout << "enter filename load from: " << std::endl;
                std::string filename;
                std::cin >> filename;
                tableSingletonInstance.loadFromFile(filename);
            }
        }
        float dt = frameClock.restart().asSeconds();
        tableSingletonInstance.update(dt, mousePosition, radius);

        tableSingletonInstance.prepDraw();
        window.clear();
        window.draw(tableSingletonInstance);
        fps++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1)
        {
            std::cout << "fps = " << fps << "\n";
            fpsClock.restart();
            fps = 0;
        }
        sf::CircleShape shape;
        shape.setRadius(radius / (float)tableSingletonInstance.getSize());
        shape.setOrigin(sf::Vector2f(1, 1) * shape.getRadius());
        shape.setPosition(mousePositionOrig);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(0.001);
        shape.setOutlineColor(sf::Color::White);
        window.draw(shape);
        window.display();
    }

    return 0;
}
