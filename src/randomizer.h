#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <random>
#include <type_traits>

namespace Randomizer
{

template <typename T>
T getRandomIntValue(T _min, T _max)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<T> distribution(_min, _max);
    return distribution(generator);
}

template <typename T>
T getRandomRealValue(T _min, T _max)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_real_distribution<T> distribution(_min, _max);
    return distribution(generator);
}

}

#endif // RANDOMIZER_H
