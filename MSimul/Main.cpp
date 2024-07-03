#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <random>

std::mt19937 rng(228);
const float FRAMERATE_RELEASE = 1.0f / 500.0f;
const float FRAMERATE_DEBUG = 1.0f / 300.0f;
const float FRAMERATE = FRAMERATE_DEBUG;

class TableSingleton : public sf::Drawable
{
private:

    static const int m_size = 300;
    sf::VertexArray m_vertexArray;
    sf::Color m_colors[256];
    float m_elapsed = 0;

    unsigned char m_elements[m_size][m_size];
    unsigned char m_newElements[m_size][m_size];

    TableSingleton() : m_vertexArray(sf::Quads, 4 * m_size * m_size)
    {
        m_colors[0] = sf::Color::Black;
        m_colors[1] = sf::Color::Blue;
        m_colors[2] = sf::Color(100, 100, 100);
        m_colors[3] = sf::Color(243, 58, 106);
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                sf::Color color;
                if ((i + j) % 2 == 0)
                {
                    color = sf::Color::Red;
                }
                else
                {
                    color = sf::Color::Blue;
                }
                m_vertexArray[4 * (m_size * i + j)] = sf::Vertex(sf::Vector2f(i, j), color);
                m_vertexArray[4 * (m_size * i + j) + 1] = sf::Vertex(sf::Vector2f(i, j + 1), color);
                m_vertexArray[4 * (m_size * i + j) + 2] = sf::Vertex(sf::Vector2f(i + 1, j + 1), color);
                m_vertexArray[4 * (m_size * i + j) + 3] = sf::Vertex(sf::Vector2f(i + 1, j), color);
            }
        }
    }
    std::vector<int> cols;
public:

    void update(float dt, sf::Vector2f mousePosition, bool isLeftMouseButtonPressed, float radius)
    {
        if (isLeftMouseButtonPressed)
        {
            // to be optimized
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
                            }
                            //m_elements[x][y] = 1;
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
                            assert(m_newElements[x][y - 1] == 0);
                            m_newElements[x][y - 1] = m_elements[x][y];
                            continue;
                        }
                        if (x + 1 < m_size && m_newElements[x + 1][y-1] == 0 && m_elements[x + 1][y-1] == 0)
                        {
                            assert(m_newElements[x + 1][y-1] == 0);
                            m_newElements[x + 1][y-1] = m_elements[x][y];
                            continue;
                        }
                        if (x - 1 >= 0 && m_newElements[x - 1][y - 1] == 0 && m_elements[x - 1][y - 1] == 0)
                        {
                            assert(m_newElements[x - 1][y - 1] == 0);
                            m_newElements[x - 1][y - 1] = m_elements[x][y];
                            continue;
                        }
                        if (rng() & 1)
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
                        if (m_newElements[x][y - 1])
                        {
                            if (rng() % 10 <= 2 ||1)
                            {
                                m_newElements[x][y - 1] = 0;
                            }
                            else
                            {
                                m_newElements[x][y - 1] = m_elements[x][y];
                            }
                            continue;
                        }
                        assert(m_newElements[x][y - 1] == 0);
                        m_newElements[x][y - 1] = m_elements[x][y];
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
                            continue;
                        }
                        assert(m_newElements[x][y] == 0);
                        m_newElements[x][y] = m_elements[x][y];
                        continue;
                    }
                }
            }

            for (int i = 0; i < m_size; i++)
            {
                for (int j = 0; j < m_size; j++)
                {
                    m_elements[i][j] = m_newElements[i][j];
                    sf::Color color = m_colors[m_elements[i][j]];
                    m_vertexArray[4 * (m_size * i + j)].color = color;
                    m_vertexArray[4 * (m_size * i + j) + 1].color = color;
                    m_vertexArray[4 * (m_size * i + j) + 2].color = color;
                    m_vertexArray[4 * (m_size * i + j) + 3].color = color;
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


int main()
{
    TableSingleton& tableSingletonInstance = TableSingleton::getInstance();

    sf::RenderWindow window(sf::VideoMode(900, 900), "MSimul");
    sf::View view;
    view.setSize(sf::Vector2f(tableSingletonInstance.getSize(), -tableSingletonInstance.getSize()));
    view.setCenter(sf::Vector2f(tableSingletonInstance.getSize(), tableSingletonInstance.getSize()) * 0.5f);
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

        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());

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
                    // event.mouseWheelScroll.delta
                    radius *= pow(1.1, event.mouseWheelScroll.delta);
                }
            }
        }
        float dt = frameClock.restart().asSeconds();
        tableSingletonInstance.update(dt, mousePosition, sf::Mouse::isButtonPressed(sf::Mouse::Left), radius);

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
        shape.setRadius(radius);
        shape.setOrigin(sf::Vector2f(1, 1) * radius);
        shape.setPosition(mousePosition);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(0.1);
        shape.setOutlineColor(sf::Color::White);
        window.draw(shape);
        window.display();
    }

    return 0;
}
