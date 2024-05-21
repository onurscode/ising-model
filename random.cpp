#include "random.h"

RandomZeroOne::RandomZeroOne() :
        rd(std::random_device()),
        mt(std::mt19937(rd())),
        distribution(std::uniform_int_distribution<>(0, 1)) {

}

RandomZeroOne::RandomZeroOne(int seed) :
        mt(std::mt19937(seed)),
        distribution(std::uniform_int_distribution<>(0, 1)) {

}

RandomBetweenZeroOne::RandomBetweenZeroOne() :
        rd(std::random_device()),
        mt(std::mt19937(rd())),
        distribution(std::uniform_real_distribution<>(0, 1)) {

}

RandomBetweenZeroOne::RandomBetweenZeroOne(int seed) :
        mt(std::mt19937(seed)),
        distribution(std::uniform_real_distribution<>(0, 1)) {

}


int RandomZeroOne::get() {
    return distribution(mt);
}

double RandomBetweenZeroOne::get() {
    return distribution(mt);
}
