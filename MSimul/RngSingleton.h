#pragma once
#include <random>

class RngSingleton 
{
private:
    std::mt19937 m_rng; 

    RngSingleton();

public:
    // returns a random unsigned int 
    unsigned int getRawRandom();

    // for singleton structure:
    RngSingleton(const RngSingleton&) = delete;
    RngSingleton& operator = (const RngSingleton&) = delete;
    static RngSingleton& getInstance();
};
