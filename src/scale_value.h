#ifndef SCALE_VALUE_H
#define SCALE_VALUE_H

template<typename T>
struct ScaleValue
{
    ScaleValue(T _val = 0) : current(_val), max(_val) {}
    void set(T _val) { current = _val; max = _val; }

    T current;
    T max;
};

#endif // SCALE_VALUE_H
