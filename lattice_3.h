#pragma once

#include <vector>
#include <utility>
#include <tuple>
#include "random.h"

class Lattice {
private:
    RandomZeroOne *randomZeroOne;
    RandomBetweenZeroOne *randomBetweenZeroOne;
    std::vector<std::vector<std::vector<int>>> lattice;
    int spinSum;
    std::vector<std::vector<std::vector<std::vector<std::tuple<int, int, int>>>>> bonds;
    double betaJ;
    bool betaJIsSet;
    double bondingProbability;
    bool thereMayBeBonds;
    void invertSpin(int i, int j, int k);

public:
    const int length, size;
    Lattice(int length, RandomZeroOne *randomZeroOne, RandomBetweenZeroOne *randomBetweenZeroOne);
    int getSpinSum();
    void setBetaJ(double betaJ);
    double getBetaJ();
    double getBondingProbability();
    void breakAllBonds();
    void formSomeBonds();
    void invertSomeClusters();
};
