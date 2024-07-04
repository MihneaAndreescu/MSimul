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
    float m_elapsed = 0;

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

public:

    // set and get for each cell
    void setCellElement(int x, int y, char type);
    char getCellElement(int x, int y);

    // for saving / loading progress
    void saveToFile(std::string filename);
    void loadFromFile(std::string filename);

    void updatePhysics();
    void update(float dt, sf::Vector2f mousePosition, float radius);

    int getSize();

    // for drawing:
    void prepDraw();
    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const;

    // for singleton structure:
    ParticlePhysicsTableSingleton(const ParticlePhysicsTableSingleton&) = delete;
    ParticlePhysicsTableSingleton& operator = (const ParticlePhysicsTableSingleton&) = delete;
    static ParticlePhysicsTableSingleton& getInstance();
};
