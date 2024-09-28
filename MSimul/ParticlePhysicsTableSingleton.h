#pragma once

#include "RngSingleton.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <random>
#include <cstdio>
#include <fstream>

class ParticlePhysicsTableSingleton : public sf::Drawable
{
private:
    static const float FRAMERATE_RELEASE;
    static const float FRAMERATE_DEBUG;
    static const float FRAMERATE;
    static const int m_pushBucketSize = 5;
    static const int m_size = 200;

    sf::VertexArray m_vertexArray;
    sf::Color m_colors[256];
    std::vector<std::pair<int, int>> m_glassCells;
    float m_elapsed = 0;
    int m_glass_x = 50;
    int m_glass_y = 50;

    std::vector<float> m_historic;

    void mv(int x1, int y1, int x2, int y2, int dimx, int dimy);

    const int m_dimx = 6;
    const int m_dimy = 20;
    unsigned char m_elements[m_size][m_size];
    unsigned char m_extra0[m_size][m_size];

    unsigned char m_newElements[m_size][m_size];
    unsigned char m_newExtra0[m_size][m_size];

    unsigned char m_direction[m_size / m_pushBucketSize][m_size / m_pushBucketSize];

    RngSingleton& m_rngSingletonInstance;

    ParticlePhysicsTableSingleton();

    std::vector<int> m_ysForReverse;

    void explodeBomb(int x, int y, int radius);
    void generateForce(int x, int y);

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

public:

    // set and get for each cell
    void setCellElement(int x, int y, char type);
    char getCellElement(int x, int y);

    // for saving / loading progress
    void saveToFile(std::string filename);
    void loadFromFile(std::string filename);

    void updatePhysics();
    void update(float dt, sf::Vector2f mousePosition, float radius, bool isGlass);

    int getSize();

    // for drawing:
    void prepDraw();
    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const;

    // for singleton structure:
    ParticlePhysicsTableSingleton(const ParticlePhysicsTableSingleton&) = delete;
    ParticlePhysicsTableSingleton& operator = (const ParticlePhysicsTableSingleton&) = delete;
    static ParticlePhysicsTableSingleton& getInstance();
};
