#pragma once

#include <random>

class RandomZeroOne {
private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<> distribution;

public:
    RandomZeroOne();
    RandomZeroOne(int seed);

    int get();
};

class RandomBetweenZeroOne {
private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<> distribution;

public:
    RandomBetweenZeroOne();
    RandomBetweenZeroOne(int seed);

    double get();
};
