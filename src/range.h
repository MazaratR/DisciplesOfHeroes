#ifndef RANGE_H
#define RANGE_H

template<typename T>
struct Range
{
    Range(T _min = 0, T _max = 0) : min(_min), max(_max) {}

    void set(T _min, T _max){
        min = _min;
        max = _max;
    }

    T min;
    T max;
};

#endif // RANGE_H
