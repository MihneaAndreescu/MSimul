#include "RngSingleton.h"
#include <chrono>

RngSingleton::RngSingleton() :
    m_rng(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{

}

RngSingleton& RngSingleton::getInstance()
{
    static RngSingleton instance;
    return instance;
}

unsigned int RngSingleton::getRawRandom()
{
    return m_rng();
}