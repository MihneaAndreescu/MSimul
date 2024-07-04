#include "RngSingleton.h"

RngSingleton::RngSingleton() :
    m_rng(std::chrono::high_resolution_clock::now().time_since_epoch().count())
{

}

RngSingleton& RngSingleton::getInstance()
{
    static RngSingleton instance;
    return instance;
}

unsigned int RngSingleton::getRandom()
{
    return m_rng();
}