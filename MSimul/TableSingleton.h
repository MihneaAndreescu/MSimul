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

    TableSingleton() : m_vertexArray(sf::Quads, 4 * m_size * m_size)
    {
        m_colors[0] = sf::Color::Black;
        m_colors[1] = sf::Color::Blue;
        m_colors[2] = sf::Color(100, 100, 100);
        m_colors[3] = sf::Color(243, 58, 106);
        m_colors[4] = sf::Color::Cyan;
        m_colors[5] = sf::Color(0, 0, 150);
        m_colors[6] = sf::Color(150, 75, 0);
        m_colors[7] = sf::Color(255, 121, 0);
        m_colors[8] = sf::Color(200, 200, 200);
        m_colors[9] = sf::Color::Red;
        m_colors[255] = sf::Color::Red;
        float inv_size = 1.0f / (float)m_size;
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                m_elements[i][j] = 0;
                m_extra0[i][j] = 0;
                sf::Color color;
                if ((i + j) % 2 == 0)
                {
                    color = sf::Color::Red;
                }
                else
                {
                    color = sf::Color::Blue;
                }
                m_vertexArray[4 * (m_size * i + j)] = sf::Vertex(sf::Vector2f(i, j)*inv_size, color);
                m_vertexArray[4 * (m_size * i + j) + 1] = sf::Vertex(sf::Vector2f(i, j + 1)*inv_size, color);
                m_vertexArray[4 * (m_size * i + j) + 2] = sf::Vertex(sf::Vector2f(i + 1, j + 1)*inv_size, color);
                m_vertexArray[4 * (m_size * i + j) + 3] = sf::Vertex(sf::Vector2f(i + 1, j)*inv_size, color);
            }
        }
    }
    std::vector<int> cols;
public:

    void saveToFile(std::string filename)
    {
        std::ofstream fileOut(filename);
        if (!fileOut) 
        {
            std::cerr << "Failed to open file for writing.\n";
            return;
        }
        for (int i = 0; i < m_size; i++) 
        {
            for (int j = 0; j < m_size; j++) 
            {
                fileOut << (char)m_elements[i][j];
            }
        }
        for (int i = 0; i < m_size; i++) 
        {
            for (int j = 0; j < m_size; j++) 
            {
                fileOut << (char)m_extra0[i][j];
            }
        }
        fileOut << '\n';
        if (!fileOut.good()) 
        {
            std::cerr << "Error occurred while writing to file.\n";
        }
    }

    void loadFromFile(std::string filename)
    {
        std::ifstream fileIn(filename);

        if (!fileIn) 
        {
            std::cerr << "Failed to open file for reading.\n";
            return;
        }

        std::string line;
        std::getline(fileIn, line);
        
        if ((int)line.size() != 2 * m_size * m_size) 
        {
            std::cerr << "File content does not match expected size.\n";
            return;
        }

        int y = 0;
        for (int i = 0; i < m_size; i++) 
        {
            for (int j = 0; j < m_size; j++) 
            {
                m_elements[i][j] = line[y++];
            }
        }

        for (int i = 0; i < m_size; i++) 
        {
            for (int j = 0; j < m_size; j++) 
            {
                m_extra0[i][j] = line[y++];
            }
        }
    }

    void bomb(int x, int y, int radius)
    {
        for (int i = x - radius; i <= x + radius; i++)
        {
            for (int j = y - radius; j <= y + radius; j++)
            {
                if (0 <= i && i < m_size && 0 <= j && j < m_size)
                {
                    if ((i - x) * (i - x) + (j - y) * (j - y) <= radius * radius)
                    {
                        if (m_elements[i][j] == 0)
                        {
                            m_newElements[i][j] = 255;
                        }
                    }
                }
            }
        }
    }

    void update(float dt, sf::Vector2f mousePosition, float radius)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for (int x = mousePosition.x - radius; x <= mousePosition.x + radius; x++)
            {
                for (int y = mousePosition.y - radius; y <= mousePosition.y + radius; y++)
                {
                    if (0 <= x && x < m_size && 0 <= y && y < m_size)
                    {
                        if ((x - mousePosition.x) * (x - mousePosition.x) + (y - mousePosition.y) * (y - mousePosition.y) <= radius * radius)
                        {
                            if (m_elements[x][y] == 0)
                            {
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
                                {
                                    m_elements[x][y] = 1;
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
                                {
                                    m_elements[x][y] = 2;
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
                                {
                                    m_elements[x][y] = 3;
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
                                {
                                    m_elements[x][y] = 4;
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5))
                                {
                                    m_elements[x][y] = 5;
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6))
                                {
                                    m_elements[x][y] = 6;
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8))
                                {
                                    m_elements[x][y] = 8;
                                }
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9))
                                {
                                    m_elements[x][y] = 9;
                                }
                            }
                            if (m_elements[x][y] == 0 || m_elements[x][y] == 7)
                            {
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7))
                                {
                                    m_extra0[x][y] = 0;
                                    m_elements[x][y] = 7;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            for (int x = mousePosition.x - radius; x <= mousePosition.x + radius; x++)
            {
                for (int y = mousePosition.y - radius; y <= mousePosition.y + radius; y++)
                {
                    if (0 <= x && x < m_size && 0 <= y && y < m_size)
                    {
                        if ((x - mousePosition.x) * (x - mousePosition.x) + (y - mousePosition.y) * (y - mousePosition.y) <= radius * radius)
                        {
                            m_elements[x][y] = 0;
                        }
                    }
                }
            }
        }
        m_elapsed += dt;

        while (m_elapsed >= FRAMERATE)
        {
            m_elapsed -= FRAMERATE;
            for (int i = 0; i < m_size; i++)
            {
                for (int j = 0; j < m_size; j++)
                {
                    m_newElements[i][j] = 0;
                    m_newExtra0[i][j] = 0;
                }
            }
            for (int y = 0; y < m_size; y++)
            {
                cols.clear();
                for (int x = 0; x < m_size; x++)
                {
                    if (m_elements[x][y] == 1)
                    {
                        if (y == 0)
                        {
                            continue;
                        }
                        assert(y - 1 >= 0);
                        if (m_elements[x][y - 1] == 0 && m_newElements[x][y - 1] == 0)
                        {
                            if (getRandom() % 5 != 0)
                            {
                                m_newElements[x][y] = m_elements[x][y];
                                continue;
                            }
                            assert(m_newElements[x][y - 1] == 0);
                            m_newElements[x][y - 1] = m_elements[x][y];
                            continue;
                        }
                        if (m_elements[x][y - 1] == 3 && m_newElements[x][y - 1] == 3)
                        {
                            m_newElements[x][y - 1] = 5;
                            continue;
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y - 1] == 0 && m_elements[x + 1][y - 1] == 0)
                            {
                                assert(m_newElements[x + 1][y - 1] == 0);
                                m_newElements[x + 1][y - 1] = m_elements[x][y];
                                continue;
                            }
                            if (x - 1 >= 0 && m_newElements[x - 1][y - 1] == 0 && m_elements[x - 1][y - 1] == 0)
                            {
                                assert(m_newElements[x - 1][y - 1] == 0);
                                m_newElements[x - 1][y - 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        else
                        {
                            if (x - 1 >= 0 && m_newElements[x - 1][y - 1] == 0 && m_elements[x - 1][y - 1] == 0)
                            {
                                assert(m_newElements[x - 1][y - 1] == 0);
                                m_newElements[x - 1][y - 1] = m_elements[x][y];
                                continue;
                            }
                            if (x + 1 < m_size && m_newElements[x + 1][y - 1] == 0 && m_elements[x + 1][y - 1] == 0)
                            {
                                assert(m_newElements[x + 1][y - 1] == 0);
                                m_newElements[x + 1][y - 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                            {
                                assert(m_newElements[x + 1][y] == 0);
                                m_newElements[x + 1][y] = m_elements[x][y];
                                continue;
                            }
                            cols.push_back(x);
                        }
                        else
                        {
                            cols.push_back(x);
                        }
                        continue;
                    }
                    if (m_elements[x][y] == 2)
                    {
                        assert(m_newElements[x][y] == 0);
                        m_newElements[x][y] = m_elements[x][y];
                        continue;
                    }
                    if (m_elements[x][y] == 3)
                    {
                        if (y == 0)
                        {
                            continue;
                        }
                        assert(y - 1 >= 0);
                        if (m_elements[x][y - 1] == 0 && m_newElements[x][y - 1] == 0)
                        {
                            if (getRandom() % 5 != 0)
                            {
                                m_newElements[x][y] = m_elements[x][y];
                                continue;
                            }
                            assert(m_newElements[x][y - 1] == 0);
                            m_newElements[x][y - 1] = m_elements[x][y];
                            continue;
                        }
                        if (m_elements[x][y - 1] == 2 && m_newElements[x][y - 1] == 2)
                        {
                            m_newElements[x][y - 1] = m_elements[x][y];
                            continue;
                        }
                        if (m_elements[x][y - 1] == 1 && m_newElements[x][y - 1] == 1)
                        {
                            m_newElements[x][y - 1] = 5;
                            continue;
                        }
                        if (m_elements[x][y - 1] == 6 && m_newElements[x][y - 1] == 6)
                        {
                            m_newElements[x][y - 1] = 5;
                            continue;
                        }
                        if (x + 1 < m_size && m_newElements[x + 1][y - 1] == 0 && m_elements[x + 1][y - 1] == 0)
                        {
                            assert(m_newElements[x + 1][y - 1] == 0);
                            m_newElements[x + 1][y - 1] = m_elements[x][y];
                            continue;
                        }
                        if (x - 1 >= 0 && m_newElements[x - 1][y - 1] == 0 && m_elements[x - 1][y - 1] == 0)
                        {
                            assert(m_newElements[x - 1][y - 1] == 0);
                            m_newElements[x - 1][y - 1] = m_elements[x][y];
                            continue;
                        }
                        if (x + 1 < m_size && m_elements[x + 1][y] == 1)
                        {
                            m_elements[x + 1][y] = 5;
                            continue;
                        }
                        if (x - 1 >= 0 && m_newElements[x - 1][y] == 1)
                        {
                            m_elements[x - 1][y] = 5;
                            continue;
                        }
                        if (x + 1 < m_size && m_elements[x + 1][y] == 6)
                        {
                            m_elements[x + 1][y] = 5;
                            continue;
                        }
                        if (x - 1 >= 0 && m_newElements[x - 1][y] == 6)
                        {
                            m_elements[x - 1][y] = 5;
                            continue;
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                            {
                                assert(m_newElements[x + 1][y] == 0);
                                m_newElements[x + 1][y] = m_elements[x][y];
                                continue;
                            }
                            cols.push_back(x);
                        }
                        else
                        {
                            cols.push_back(x);
                        }
                        continue;
                    }
                    if (m_elements[x][y] == 4)
                    {
                        assert(m_newElements[x][y] == 0);
                        m_newElements[x][y] = m_elements[x][y];
                        continue;
                    }
                    if (m_elements[x][y] == 5)
                    {
                        assert(m_newElements[x][y] == 0);
                        if (y + 1 >= m_size)
                        {
                            continue;
                        }
                        if (m_elements[x][y + 1] == 1)
                        {
                            m_elements[x][y + 1] = 0;
                            m_newElements[x][y + 1] = 5;
                            m_newElements[x][y] = 5;
                            continue;
                        }
                        if (m_elements[x][y + 1] == 3)
                        {
                            m_elements[x][y + 1] = 0;
                            continue;
                        }
                        if (getRandom() % 10 != 0)
                        {
                            m_newElements[x][y] = m_elements[x][y];
                            continue;
                        }
                        assert(y + 1 < m_size);
                        if (m_elements[x][y + 1] == 0 && m_newElements[x][y + 1] == 0)
                        {
                            assert(m_newElements[x][y + 1] == 0);
                            m_newElements[x][y + 1] = m_elements[x][y];
                            continue;
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y + 1] == 0 && m_elements[x + 1][y + 1] == 0)
                            {
                                assert(m_newElements[x + 1][y + 1] == 0);
                                m_newElements[x + 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                            if (x - 1 >= 0 && m_newElements[x - 1][y + 1] == 0 && m_elements[x - 1][y + 1] == 0)
                            {
                                assert(m_newElements[x - 1][y + 1] == 0);
                                m_newElements[x - 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        else
                        {
                            if (x - 1 >= 0 && m_newElements[x - 1][y + 1] == 0 && m_elements[x - 1][y + 1] == 0)
                            {
                                assert(m_newElements[x - 1][y + 1] == 0);
                                m_newElements[x - 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                            if (x + 1 < m_size && m_newElements[x + 1][y + 1] == 0 && m_elements[x + 1][y + 1] == 0)
                            {
                                assert(m_newElements[x + 1][y + 1] == 0);
                                m_newElements[x + 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                            {
                                assert(m_newElements[x + 1][y] == 0);
                                m_newElements[x + 1][y] = m_elements[x][y];
                                continue;
                            }
                            cols.push_back(x);
                        }
                        else
                        {
                            cols.push_back(x);
                        }
                        continue;
                    }
                    if (m_elements[x][y] == 6)
                    {
                        assert(m_newElements[x][y] == 0);
                        m_newElements[x][y] = m_elements[x][y];
                        continue;
                    }
                    if (m_elements[x][y] == 7)
                    {
                        if (getRandom() % 400 == 0)
                        {
                            m_newElements[x][y] = 8;
                            continue;
                        }
                        assert(m_newElements[x][y] == 0);
                        if (getRandom() % 20 == 0)
                        {
                            if (m_extra0[x][y] + 1 <= 2)
                            {
                                m_extra0[x][y]++;
                            }
                        }
                        if (y + 1 >= m_size)
                        {
                            continue;
                        }
                        if (getRandom() % 2 == 0)
                        {
                            if (y + 1 < m_size && (m_elements[x][y + 1] == 1 || m_elements[x][y + 1] == 5))
                            {
                                continue;
                            }
                            if (x + 1 < m_size && (m_elements[x + 1][y] == 1 || m_elements[x][y + 1] == 5))
                            {
                                continue;
                            }
                            if (x - 1 >= 0 && (m_elements[x - 1][y] == 1 || m_elements[x][y + 1] == 5))
                            {
                                continue;
                            }
                        }
                        if (getRandom() % 10 != 0)
                        {
                            m_newExtra0[x][y] = m_extra0[x][y];
                            m_newElements[x][y] = m_elements[x][y];
                            continue;
                        }
                        assert(y + 1 < m_size);
                        if (m_elements[x][y + 1] == 0 && m_newElements[x][y + 1] == 0)
                        {
                            assert(m_newElements[x][y + 1] == 0);
                            m_newExtra0[x][y + 1] = m_extra0[x][y];
                            m_newElements[x][y + 1] = m_elements[x][y];
                            continue;
                        }
                        if (y - 1 >= 0 && m_newElements[x][y - 1] == 6)
                        {
                            m_newElements[x][y - 1] = (m_elements[x][y]);
                            m_newElements[x][y] = m_elements[x][y];
                            continue;
                        }
                        if (m_elements[x][y + 1] == 6)
                        {
                            m_elements[x][y + 1] = 0;
                            m_newElements[x][y + 1] = m_elements[x][y];
                            m_newElements[x][y] = m_elements[x][y];
                            continue;
                        }
                        if (x + 1 < m_size && m_elements[x + 1][y] == 6)
                        {
                            m_elements[x + 1][y] = 0;
                            m_newElements[x + 1][y] = (m_elements[x][y]);
                            m_newElements[x][y] = m_elements[x][y];
                            continue;
                        }
                        if (x - 1 >= 0 && m_newElements[x - 1][y] == 6)
                        {
                            m_newElements[x - 1][y] = (m_elements[x][y]);
                            m_newElements[x][y] = m_elements[x][y];
                            continue;
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y + 1] == 0 && m_elements[x + 1][y + 1] == 0)
                            {
                                assert(m_newElements[x + 1][y + 1] == 0);
                                m_newExtra0[x + 1][y + 1] = m_extra0[x][y];
                                m_newElements[x + 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                            if (x - 1 >= 0 && m_newElements[x - 1][y + 1] == 0 && m_elements[x - 1][y + 1] == 0)
                            {
                                assert(m_newElements[x - 1][y + 1] == 0);
                                m_newExtra0[x - 1][y + 1] = m_extra0[x][y];
                                m_newElements[x - 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        else
                        {
                            if (x - 1 >= 0 && m_newElements[x - 1][y + 1] == 0 && m_elements[x - 1][y + 1] == 0)
                            {
                                assert(m_newElements[x - 1][y + 1] == 0);
                                m_newExtra0[x - 1][y + 1] = m_extra0[x][y];
                                m_newElements[x - 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                            if (x + 1 < m_size && m_newElements[x + 1][y + 1] == 0 && m_elements[x + 1][y + 1] == 0)
                            {
                                assert(m_newElements[x + 1][y + 1] == 0);
                                m_newExtra0[x + 1][y + 1] = m_extra0[x][y];
                                m_newElements[x + 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                            {
                                assert(m_newElements[x + 1][y] == 0);
                                m_newExtra0[x + 1][y] = m_extra0[x][y];
                                m_newElements[x + 1][y] = m_elements[x][y];
                                continue;
                            }
                            cols.push_back(x);
                        }
                        else
                        {
                            cols.push_back(x);
                        }
                        continue;
                    }
                    if (m_elements[x][y] == 8)
                    {
                        if (getRandom() % 400 == 0)
                        {
                            continue;
                        }
                        assert(m_newElements[x][y] == 0);
                        if (y + 1 >= m_size)
                        {
                            continue;
                        }
                        if (getRandom() % 10 != 0)
                        {
                            m_newElements[x][y] = m_elements[x][y];
                            continue;
                        }
                        assert(y + 1 < m_size);
                        if (m_elements[x][y + 1] == 0 && m_newElements[x][y + 1] == 0)
                        {
                            assert(m_newElements[x][y + 1] == 0);
                            m_newElements[x][y + 1] = m_elements[x][y];
                            continue;
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y + 1] == 0 && m_elements[x + 1][y + 1] == 0)
                            {
                                assert(m_newElements[x + 1][y + 1] == 0);
                                m_newElements[x + 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                            if (x - 1 >= 0 && m_newElements[x - 1][y + 1] == 0 && m_elements[x - 1][y + 1] == 0)
                            {
                                assert(m_newElements[x - 1][y + 1] == 0);
                                m_newElements[x - 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        else
                        {
                            if (x - 1 >= 0 && m_newElements[x - 1][y + 1] == 0 && m_elements[x - 1][y + 1] == 0)
                            {
                                assert(m_newElements[x - 1][y + 1] == 0);
                                m_newElements[x - 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                            if (x + 1 < m_size && m_newElements[x + 1][y + 1] == 0 && m_elements[x + 1][y + 1] == 0)
                            {
                                assert(m_newElements[x + 1][y + 1] == 0);
                                m_newElements[x + 1][y + 1] = m_elements[x][y];
                                continue;
                            }
                        }
                        if (getRandom() & 1)
                        {
                            if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                            {
                                assert(m_newElements[x + 1][y] == 0);
                                m_newElements[x + 1][y] = m_elements[x][y];
                                continue;
                            }
                            cols.push_back(x);
                        }
                        else
                        {
                            cols.push_back(x);
                        }
                        continue;
                    }
                    if (m_elements[x][y] == 9 || m_elements[x][y] == 255)
                    {
                        int radius = 5;
                        if (x - 1 >= 0 && m_elements[x - 1][y] == 7)
                        {
                            if (m_elements[x][y] == 9)
                            {
                                bomb(x, y, radius);
                            }
                            m_newElements[x][y] = 7;
                            continue;
                        }
                        if (x + 1 < m_size && m_elements[x + 1][y] == 7)
                        {
                            if (m_elements[x][y] == 9)
                            {
                                bomb(x, y, radius);
                            }
                            m_newElements[x][y] = 7;
                            continue;
                        }
                        if (y - 1 >= 0 && m_elements[x][y - 1] == 7)
                        {
                            if (m_elements[x][y] == 9)
                            {
                                bomb(x, y, radius);
                            }
                            m_newElements[x][y] = 7;
                            continue;
                        }            
                        if (y + 1 < m_size && m_elements[x][y + 1] == 7)
                        {
                            if (m_elements[x][y] == 9)
                            {
                                bomb(x, y, radius);
                            }
                            m_newElements[x][y] = 7;
                            continue;
                        }
                        assert(m_newElements[x][y] == 0);
                        m_newElements[x][y] = m_elements[x][y];
                        continue;
                    }
                }
                if (!cols.empty())
                {
                    reverse(cols.begin(), cols.end());
                    for (auto& x : cols)
                    {
                        if (x - 1 >= 0 && m_newElements[x - 1][y] == 0 && m_elements[x - 1][y] == 0)
                        {
                            assert(m_newElements[x - 1][y] == 0);
                            m_newElements[x - 1][y] = m_elements[x][y];
                            m_newExtra0[x - 1][y] = m_extra0[x][y];
                            continue;
                        }
                        assert(m_newElements[x][y] == 0);
                        m_newElements[x][y] = m_elements[x][y];
                        m_newExtra0[x][y] = m_extra0[x][y];
                        continue;
                    }
                }
            }

            for (int i = 0; i < m_size; i++)
            {
                for (int j = 0; j < m_size; j++)
                {
                    m_elements[i][j] = m_newElements[i][j];
                    m_newExtra0[i][j] = m_extra0[i][j];
                }
            }
        }
    }

    int getSize()
    {
        return m_size;
    }

    TableSingleton(const TableSingleton&) = delete;

    TableSingleton& operator = (const TableSingleton&) = delete;

    static TableSingleton& getInstance()
    {
        static TableSingleton instance;
        return instance;
    }

    void prepDraw()
    {
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                sf::Color color = m_colors[m_elements[i][j]];
                if (m_elements[i][j] == 3)
                {
                    int ran = getRandom() % 2;
                    if (ran == 0)
                    {
                        color.r -= 50;
                        color.g += 50;
                        color.b -= 50;
                    }
                }
                if (m_elements[i][j] == 7)
                {
                    int type = m_extra0[i][j];
                    assert(0 <= type && type <= 2);
                    if (type == 0)
                    {
                        color = sf::Color::White;
                    }
                    if (type == 2)
                    {
                        color = sf::Color::Red;
                    }
                }
                m_vertexArray[4 * (m_size * i + j)].color = color;
                m_vertexArray[4 * (m_size * i + j) + 1].color = color;
                m_vertexArray[4 * (m_size * i + j) + 2].color = color;
                m_vertexArray[4 * (m_size * i + j) + 3].color = color;
            }
        }
    }

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
    {
        renderTarget.draw(m_vertexArray, renderStates);
    }
};
