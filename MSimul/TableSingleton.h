#pragma once

#include "Rng.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <random>
#include <cstdio>
#include <fstream>

class TableSingleton : public sf::Drawable
{
private:
    static const float FRAMERATE_RELEASE;
    static const float FRAMERATE_DEBUG;
    static const float FRAMERATE;
    static const int m_size = 200;

    sf::VertexArray m_vertexArray;
    sf::Color m_colors[256];
    float m_elapsed = 0;

    unsigned char m_elements[m_size][m_size], m_extra0[m_size][m_size];
    unsigned char m_newElements[m_size][m_size], m_newExtra0[m_size][m_size];

    TableSingleton();

    std::vector<int> cols;

public:

    void saveToFile(std::string filename);

    void loadFromFile(std::string filename);

    void explodeBomb(int x, int y, int radius);

    void update(float dt, sf::Vector2f mousePosition, float radius);

    int getSize();

    TableSingleton(const TableSingleton&) = delete;

    TableSingleton& operator = (const TableSingleton&) = delete;

    static TableSingleton& getInstance();

    void prepDraw();

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const;
};
