#pragma once
#include <random>
#include <chrono>

class RngSingleton 
{
private:
    std::mt19937 m_rng; 

    RngSingleton();

public:
    RngSingleton(const RngSingleton&) = delete;

    RngSingleton& operator = (const RngSingleton&) = delete;

    static RngSingleton& getInstance();

    unsigned int getRandom();
};
