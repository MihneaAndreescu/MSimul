#include "ParticlePhysicsTableSingleton.h"
#include <algorithm>

const float ParticlePhysicsTableSingleton::FRAMERATE_RELEASE = 1.0f / 500.0f;
const float ParticlePhysicsTableSingleton::FRAMERATE_DEBUG = 1.0f / 300.0f;
const float ParticlePhysicsTableSingleton::FRAMERATE = ParticlePhysicsTableSingleton::FRAMERATE_DEBUG;

ParticlePhysicsTableSingleton::ParticlePhysicsTableSingleton() : 
    m_vertexArray(sf::Quads, 4 * m_size * m_size),
    m_rngSingletonInstance(RngSingleton::getInstance())
{
    for (int i = 0; i < m_size / m_pushBucketSize; i++)
    {
        for (int j = 0; j < m_size / m_pushBucketSize; j++)
        {
            m_direction[i][j] = 0;
        }
    }
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
            m_vertexArray[4 * (m_size * i + j)] = sf::Vertex(sf::Vector2f(i, j) * inv_size, color);
            m_vertexArray[4 * (m_size * i + j) + 1] = sf::Vertex(sf::Vector2f(i, j + 1) * inv_size, color);
            m_vertexArray[4 * (m_size * i + j) + 2] = sf::Vertex(sf::Vector2f(i + 1, j + 1) * inv_size, color);
            m_vertexArray[4 * (m_size * i + j) + 3] = sf::Vertex(sf::Vector2f(i + 1, j) * inv_size, color);
        }
    }
}

void ParticlePhysicsTableSingleton::saveToFile(std::string filename)
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

void ParticlePhysicsTableSingleton::loadFromFile(std::string filename)
{
    std::ifstream fileIn(filename);

    if (!fileIn)
    {
        std::cerr << "Failed to open file for reading.\n";
        return;
    }

    std::string line;
    std::getline(fileIn, line);

    // commented this in order to have backwards compatibility
    //if ((int)line.size() != 2 * m_size * m_size)
    //{
    //    std::cerr << "File content does not match expected size.\n";
    //    return;
    //}

    int y = 0;

    for (int i = 0; i < m_size && y < (int)line.size(); i++)
    {
        for (int j = 0; j < m_size && y < (int)line.size(); j++)
        {
            m_elements[i][j] = line[y++];
        }
    }

    for (int i = 0; i < m_size && y < (int)line.size(); i++)
    {
        for (int j = 0; j < m_size && y < (int)line.size(); j++)
        {
            m_extra0[i][j] = line[y++];
        }
    }
}

void ParticlePhysicsTableSingleton::explodeBomb(int x, int y, int radius)
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

void ParticlePhysicsTableSingleton::generateForce(int x, int y)
{
    x /= m_pushBucketSize;
    y /= m_pushBucketSize;
    assert(0 <= x && x < m_size / m_pushBucketSize);
    assert(0 <= y && y < m_size / m_pushBucketSize);
    // 0 down
    // 1 right
    // 2 up
    // 3 left
    const int l = 10;
    for (int i = x - l + 1; i <= x + l; i++)
    {
        for (int j = y - l + 1; j <= y + l; j++)
        {
            if (0 <= i && i < m_size / m_pushBucketSize && 0 <= j && j < m_size / m_pushBucketSize)
            {
                if (i <= x)
                {
                    if (j <= y)
                    {
                        m_direction[i][j] |= (1 << 0) | (1 << 3);
                    }
                    else
                    {
                        m_direction[i][j] |= (1 << 2) | (1 << 4);
                    }
                }
                else
                {
                    if (j <= y)
                    {
                        m_direction[i][j] |= (1 << 0) | (1 << 1);
                    }
                    else
                    {
                        m_direction[i][j] |= (1 << 2) | (1 << 1);
                    }
                }
            }
        }
    }
}

void ParticlePhysicsTableSingleton::updatePhysics()
{
    sort(m_glassCells.begin(), m_glassCells.end());
    m_glassCells.resize(std::unique(m_glassCells.begin(), m_glassCells.end()) - m_glassCells.begin());
    std::vector<std::pair<std::pair<int, int>, int>> history;
    for (auto& it : m_glassCells)
    {
        int x = it.first, y = it.second;
        if (0 <= x && x < m_size && 0 <= y && y < m_size)
        {
            history.push_back({ {x, y}, m_elements[x][y] });
            m_elements[x][y] = 2;
        }
    }
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
        m_ysForReverse.clear();
        for (int x = 0; x < m_size; x++)
        {
            for (int b = 0; b < 4; b++)
            {
                if ((m_direction[x / m_pushBucketSize][y / m_pushBucketSize] & (1 << b)) && m_rngSingletonInstance.getRawRandom() % 1000 == 0)
                {
                    m_direction[x / m_pushBucketSize][y / m_pushBucketSize] ^= (1 << b);
                }
            }
            if (m_elements[x][y] == 1)
            {
                if (y == 0)
                {
                    continue;
                }
                if (y == m_size - 1)
                {
                    continue;
                }
                if (x == 0)
                {
                    continue;
                }
                if (x == m_size - 1)
                {
                    continue;
                }
                // 0 down
                // 1 right
                // 2 up
                // 3 left
                const int M = 3;
                //if (m_direction[x / m_pushBucketSize][y / m_pushBucketSize] & (1 << 0))
                //{
                //    if (m_rngSingletonInstance.getRawRandom() % M == 0)
                //    {
                //        if (m_elements[x][y - 1] == 0 && m_newElements[x][y - 1] == 0)
                //        {
                //            //m_direction[x / m_pushBucketSize][(y - 1) / m_pushBucketSize] |= (1 << 0);
                //            m_newElements[x][y - 1] = m_elements[x][y];
                //            continue;
                //        }
                //    }
                //}
                if (m_direction[x / m_pushBucketSize][y / m_pushBucketSize] & (1 << 1))
                {
                    if (m_rngSingletonInstance.getRawRandom() % M == 0)
                    {
                        if (m_elements[x + 1][y] == 0)
                        {
                            m_direction[(x+1) / m_pushBucketSize][y / m_pushBucketSize] |= (1 << 1);
                            m_newElements[x + 1][y] = m_elements[x][y];
                            continue;
                        }
                    }
                }
                if (m_direction[x / m_pushBucketSize][y / m_pushBucketSize] & (1 << 2))
                {
                    if (m_rngSingletonInstance.getRawRandom() % M == 0)
                    {
                        if (m_elements[x][y + 1] == 0)
                        {
                            m_direction[x / m_pushBucketSize][(y+1) / m_pushBucketSize] |= (1 << 2);
                            m_newElements[x][y + 1] = m_elements[x][y];
                            continue;
                        }
                    }
                }
                if (m_direction[x / m_pushBucketSize][y / m_pushBucketSize] & (1 << 3))
                {
                    if (m_rngSingletonInstance.getRawRandom() % M == 0)
                    {
                        if (m_elements[x - 1][y] == 0 && m_newElements[x - 1][y] == 0)
                        {
                            m_direction[(x - 1) / m_pushBucketSize][y / m_pushBucketSize] |= (1 << 3);
                            m_newElements[x - 1][y] = m_elements[x][y];
                            continue;
                        }
                    }
                }
                //if (m_elements[x][y] && m_rngSingletonInstance.getRawRandom() % 10 == 0)
                //{
                //    if (m_elements[x + 1][y] == 0)
                //    {
                //        m_newElements[x + 1][y] = m_elements[x][y];
                //        continue;
                //    }
                //}
                assert(y - 1 >= 0);
                if (m_elements[x][y - 1] == 0 && m_newElements[x][y - 1] == 0)
                {
                    if (m_rngSingletonInstance.getRawRandom() % 5 != 0)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
                {
                    if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                    {
                        assert(m_newElements[x + 1][y] == 0);
                        m_newElements[x + 1][y] = m_elements[x][y];
                        continue;
                    }
                    m_ysForReverse.push_back(x);
                }
                else
                {
                    m_ysForReverse.push_back(x);
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
                if (y == m_size - 1)
                {
                    continue;
                }
                if (x == 0)
                {
                    continue;
                }
                if (x == m_size - 1)
                {
                    continue;
                }
                assert(y - 1 >= 0);
                if (m_elements[x][y - 1] == 0 && m_newElements[x][y - 1] == 0)
                {
                    if (m_rngSingletonInstance.getRawRandom() % 5 != 0)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
                {
                    if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                    {
                        assert(m_newElements[x + 1][y] == 0);
                        m_newElements[x + 1][y] = m_elements[x][y];
                        continue;
                    }
                    m_ysForReverse.push_back(x);
                }
                else
                {
                    m_ysForReverse.push_back(x);
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
                if (m_rngSingletonInstance.getRawRandom() % 10 != 0)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
                {
                    if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                    {
                        assert(m_newElements[x + 1][y] == 0);
                        m_newElements[x + 1][y] = m_elements[x][y];
                        continue;
                    }
                    m_ysForReverse.push_back(x);
                }
                else
                {
                    m_ysForReverse.push_back(x);
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
                if (m_rngSingletonInstance.getRawRandom() % 400 == 0)
                {
                    m_newElements[x][y] = 8;
                    continue;
                }
                assert(m_newElements[x][y] == 0);
                if (m_rngSingletonInstance.getRawRandom() % 20 == 0)
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
                if (m_rngSingletonInstance.getRawRandom() % 2 == 0)
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
                if (m_rngSingletonInstance.getRawRandom() % 10 != 0)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
                {
                    if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                    {
                        assert(m_newElements[x + 1][y] == 0);
                        m_newExtra0[x + 1][y] = m_extra0[x][y];
                        m_newElements[x + 1][y] = m_elements[x][y];
                        continue;
                    }
                    m_ysForReverse.push_back(x);
                }
                else
                {
                    m_ysForReverse.push_back(x);
                }
                continue;
            }
            if (m_elements[x][y] == 8)
            {
                if (m_rngSingletonInstance.getRawRandom() % 400 == 0)
                {
                    continue;
                }
                assert(m_newElements[x][y] == 0);
                if (y + 1 >= m_size)
                {
                    continue;
                }
                if (m_rngSingletonInstance.getRawRandom() % 10 != 0)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
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
                if (m_rngSingletonInstance.getRawRandom() & 1)
                {
                    if (x + 1 < m_size && m_newElements[x + 1][y] == 0 && m_elements[x + 1][y] == 0)
                    {
                        assert(m_newElements[x + 1][y] == 0);
                        m_newElements[x + 1][y] = m_elements[x][y];
                        continue;
                    }
                    m_ysForReverse.push_back(x);
                }
                else
                {
                    m_ysForReverse.push_back(x);
                }
                continue;
            }
            if (m_elements[x][y] == 9 || m_elements[x][y] == 255)
            {
                int radius = 5;
                if (x - 1 >= 0 && (m_elements[x - 1][y] == 7 || m_elements[x - 1][y] == 3))
                {
                    if (m_elements[x][y] == 9)
                    {
                        explodeBomb(x, y, radius);
                    }

                    generateForce(x, y);

                    m_newElements[x][y] = 7;
                    continue;
                }
                if (x + 1 < m_size && (m_elements[x + 1][y] == 7 || m_elements[x + 1][y] == 3))
                {
                    if (m_elements[x][y] == 9)
                    {
                        explodeBomb(x, y, radius);
                    }

                    generateForce(x, y);

                    m_newElements[x][y] = 7;
                    continue;
                }
                if (y - 1 >= 0 && (m_elements[x][y - 1] == 7 || m_elements[x][y - 1] == 3))
                {
                    if (m_elements[x][y] == 9)
                    {
                        explodeBomb(x, y, radius);
                    }
                    

                    generateForce(x, y);

                    m_newElements[x][y] = 7;
                    continue;
                }
                if (y + 1 < m_size && (m_elements[x][y + 1] == 7 || m_elements[x][y + 1] == 3))
                {
                    if (m_elements[x][y] == 9)
                    {
                        explodeBomb(x, y, radius);
                    }
                    

                    generateForce(x, y);

                    m_newElements[x][y] = 7;
                    continue;
                }
                assert(m_newElements[x][y] == 0);
                m_newElements[x][y] = m_elements[x][y];
                continue;
            }
        }
        if (!m_ysForReverse.empty())
        {
            reverse(m_ysForReverse.begin(), m_ysForReverse.end());
            for (auto& x : m_ysForReverse)
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
    for (auto& it : history)
    {
        int x = it.first.first, y = it.first.second, oldValue = it.second;
        m_elements[x][y] = oldValue;
    }
}

void ParticlePhysicsTableSingleton::setCellElement(int x, int y, char type)
{
    assert(0 <= x && x < m_size);
    assert(0 <= y && y < m_size);
    m_elements[x][y] = type;
    m_extra0[x][y] = 0;
}

char ParticlePhysicsTableSingleton::getCellElement(int x, int y)
{
    assert(0 <= x && x < m_size);
    assert(0 <= y && y < m_size);
    return m_elements[x][y];
}

#include <iostream>

void ParticlePhysicsTableSingleton::mv(int x1, int y1, int x2, int y2, int dimx, int dimy)
{
    for (int x = 0; x < m_size; x++)
    {
        for (int y = 0; y < m_size; y++)
        {
            m_newElements[x][y] = m_elements[x][y];
            m_newExtra0[x][y] = m_extra0[x][y];
        }
    }
    //std::cout << x1 << "..." << x1 + dimx - 1 << " and " << y1 << "..." << y1 + dimy - 1 << " ---> ";
    //for (int i = 0; i < m_size; i++)
    //{
    //    for (int j = 0; j < m_size; j++)
    //    {
    //        if (m_elements[i][j] == 1)
    //        {
    //            if (!(x1 <= i && i < x1 + dimx && y1 <= j && j < y1 + dimy))
    //            {
    //                std::cout << "error = " << i << " " << j << "\n";
    //            }
    //        }
    //    }
    //}
    //std::cout << "move is happening\n";
    //std::cout << "\n";
    for (int dx = 0; dx < dimx; dx++)
    {
        for (int dy = 0; dy < dimy; dy++)
        {
            int r1 = x1 + dx, c1 = y1 + dy;
            int r2 = x2 + dx, c2 = y2 + dy;
            if (0 <= r1 && r1 < m_size && 0 <= c1 && c1 < m_size)
            {
                //std::cout << (int) m_elements[r1][c1] << " ";
                m_newElements[r1][c1] = 0;
                //m_newExtra0[r1][c1] = 0;


                //m_newExtra0[r1][c1] = 77;
            }
        }
    }
    for (int dx = 0; dx < dimx; dx++)
    {
        for (int dy = 0; dy < dimy; dy++)
        {
            if (0 <= x1 + dx && x1 + dx < m_size && 0 <= y1 + dy && y1 + dy < m_size)
            {
                if (0 <= x2 + dx && x2 + dx < m_size && 0 <= y2 + dy && y2 + dy < m_size)
                {
                    m_newElements[x2 + dx][y2 + dy] = m_elements[x1 + dx][y1 + dy];
                    m_newExtra0[x2 + dx][y2 + dy] = m_extra0[x1 + dx][y1 + dy];
                }
            }
        }
        //std::cout << "\n";
    }
    for (int x = 0; x < m_size; x++)
    {
        for (int y = 0; y < m_size; y++)
        {
            m_elements[x][y] = m_newElements[x][y];
            m_extra0[x][y] = m_newExtra0[x][y];
        }
    }

}

void ParticlePhysicsTableSingleton::moveLeft()
{
    int nwx = m_glass_x - 1, nwy = m_glass_y;
    mv(m_glass_x - m_dimx, m_glass_y, nwx - m_dimx, nwy, 2 * m_dimx + 1, m_dimy + 1);
    m_glass_x = nwx;
    m_glass_y = nwy;
}

void ParticlePhysicsTableSingleton::moveRight()
{
    int nwx = m_glass_x + 1, nwy = m_glass_y;
    mv(m_glass_x - m_dimx, m_glass_y, nwx - m_dimx, nwy, 2 * m_dimx + 1, m_dimy + 1);
    m_glass_x = nwx;
    m_glass_y = nwy;
}

void ParticlePhysicsTableSingleton::moveUp()
{
    int nwx = m_glass_x, nwy = m_glass_y + 1;
    mv(m_glass_x - m_dimx, m_glass_y, nwx - m_dimx, nwy, 2 * m_dimx + 1, m_dimy + 1);
    m_glass_x = nwx;
    m_glass_y = nwy;
}

void ParticlePhysicsTableSingleton::moveDown()
{
    int nwx = m_glass_x, nwy = m_glass_y - 1;
    mv(m_glass_x - m_dimx, m_glass_y, nwx - m_dimx, nwy, 2 * m_dimx + 1, m_dimy + 1);
    m_glass_x = nwx;
    m_glass_y = nwy;
}

void ParticlePhysicsTableSingleton::update(float dt, sf::Vector2f mousePosition, float radius, bool isGlass)
{
    m_glassCells.clear();
    if (isGlass)
    {
        //int x = mousePosition.x, y = mousePosition.y;
        for (int j = 0; j <= m_dimx; j++)
        {
            m_glassCells.push_back({ m_glass_x - j, m_glass_y });
            m_glassCells.push_back({ m_glass_x + j, m_glass_y });
        }
        for (int j = 0; j <= m_dimy; j++)
        {
            m_glassCells.push_back({ m_glass_x - m_dimx, m_glass_y + j });
            m_glassCells.push_back({ m_glass_x + m_dimx, m_glass_y + j });
        }
        //std::cout << " = " << mousePosition.x << " " << mousePosition.y << "\n";
    }
    if (m_historic.empty())
    {
        m_historic.push_back(0);
        m_historic.push_back(0);
        m_historic.push_back(0);
        m_historic.push_back(0);
    }
    assert((int)m_historic.size() == 4);
    const float coolDown = 0.01;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_historic[0] <= 0)
    {
        moveLeft();
        m_historic[0] = coolDown;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_historic[1] <= 0)
    {
        moveRight();
        m_historic[1] = coolDown;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_historic[2] <= 0)
    {
        moveUp();
        m_historic[2] = coolDown;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_historic[3] <= 0)
    {
        moveDown();
        m_historic[3] = coolDown;
    }
    for (auto& times : m_historic)
    {
        times -= dt;
    }
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
        updatePhysics();
    }

}

int ParticlePhysicsTableSingleton::getSize()
{
    return m_size;
}

ParticlePhysicsTableSingleton& ParticlePhysicsTableSingleton::getInstance()
{
    static ParticlePhysicsTableSingleton instance;
    return instance;
}

void ParticlePhysicsTableSingleton::prepDraw()
{
    for (int i = 0; i < m_size; i++)
    {
        for (int j = 0; j < m_size; j++)
        {
            sf::Color color = m_colors[m_elements[i][j]];
            if (m_elements[i][j] == 3)
            {
                int ran = m_rngSingletonInstance.getRawRandom() % 2;
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
            if (m_extra0[i][j] == 77)
            {
                color = sf::Color::Yellow;
            }
            m_vertexArray[4 * (m_size * i + j)].color = color;
            m_vertexArray[4 * (m_size * i + j) + 1].color = color;
            m_vertexArray[4 * (m_size * i + j) + 2].color = color;
            m_vertexArray[4 * (m_size * i + j) + 3].color = color;
        }
    }
    for (auto& it : m_glassCells)
    {
        int i = it.first, j = it.second;
        if (0 <= i && i < m_size && 0 <= j && j < m_size)
        {
            sf::Color color = sf::Color::Yellow;
            m_vertexArray[4 * (m_size * i + j)].color = color;
            m_vertexArray[4 * (m_size * i + j) + 1].color = color;
            m_vertexArray[4 * (m_size * i + j) + 2].color = color;
            m_vertexArray[4 * (m_size * i + j) + 3].color = color;
        }
    }
}

void ParticlePhysicsTableSingleton::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
{
    renderTarget.draw(m_vertexArray, renderStates);
}