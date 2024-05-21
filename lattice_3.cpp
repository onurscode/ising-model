#include "lattice_3.h"
#include <cmath>
#include <queue>

Lattice::Lattice(int length, RandomZeroOne *randomZeroOne, RandomBetweenZeroOne *randomBetweenZeroOne) :
        lattice(std::vector<std::vector<std::vector<int>>>(length, std::vector<std::vector<int>>(length, std::vector<int>(length, 1)))),
        bonds(std::vector<std::vector<std::vector<std::vector<std::tuple<int, int, int>>>>>(length, std::vector<std::vector<std::vector<std::tuple<int, int, int>>>>(length, std::vector<std::vector<std::tuple<int, int, int>>>(length, std::vector<std::tuple<int, int, int>>())))),
        length(length),
        size(length * length * length) {
    spinSum = size;
    betaJIsSet = false;
    thereMayBeBonds = false;
    this->randomZeroOne = randomZeroOne;
    this->randomBetweenZeroOne = randomBetweenZeroOne;
}

int Lattice::getSpinSum() {
    return spinSum;
}

void Lattice::setBetaJ(double betaJ) {
    this->betaJ = betaJ;
    betaJIsSet = true;
    bondingProbability = 1 - std::exp(-2 * betaJ);
}

class BetaJIsNotSetException {};

double Lattice::getBetaJ() {
    if (!betaJIsSet) throw BetaJIsNotSetException();
    return betaJ;
}

double Lattice::getBondingProbability() {
    if (!betaJIsSet) throw BetaJIsNotSetException();
    return bondingProbability;
}

void Lattice::breakAllBonds() {
    bonds = std::vector<std::vector<std::vector<std::vector<std::tuple<int, int, int>>>>>(length, std::vector<std::vector<std::vector<std::tuple<int, int, int>>>>(length, std::vector<std::vector<std::tuple<int, int, int>>>(length, std::vector<std::tuple<int, int, int>>())));
    thereMayBeBonds = false;
}

class ThereMayBeBondsException {};

void Lattice::formSomeBonds() {
    if (!betaJIsSet) throw BetaJIsNotSetException();
    if (thereMayBeBonds) throw ThereMayBeBondsException();
    thereMayBeBonds = true;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            for (int k = 0; k < length; k++) {
                int i_next = (i == length - 1) ? 0 : i + 1;
                int j_next = (j == length - 1) ? 0 : j + 1;
                int k_next = (k == length - 1) ? 0 : k + 1;
                if (lattice[i][j][k] == lattice[i_next][j][k]) {
                    if (randomBetweenZeroOne->get() < bondingProbability) {
                        bonds[i][j][k].push_back(std::make_tuple(i_next, j, k));
                        bonds[i_next][j][k].push_back(std::make_tuple(i, j, k));
                    }
                }
                if (lattice[i][j][k] == lattice[i][j_next][k]) {
                    if (randomBetweenZeroOne->get() < bondingProbability) {
                        bonds[i][j][k].push_back(std::make_tuple(i, j_next, k));
                        bonds[i][j_next][k].push_back(std::make_tuple(i, j, k));
                    }
                }
                if (lattice[i][j][k] == lattice[i][j][k_next]) {
                    if (randomBetweenZeroOne->get() < bondingProbability) {
                        bonds[i][j][k].push_back(std::make_tuple(i, j, k_next));
                        bonds[i][j][k_next].push_back(std::make_tuple(i, j, k));
                    }
                }
            }
        }
    }
}

void Lattice::invertSpin(int i, int j, int k) {
    if (lattice[i][j][k] == 1) {
        lattice[i][j][k] = -1;
        spinSum -= 2;
    } else {
        lattice[i][j][k] = 1;
        spinSum += 2;
    }
}

void Lattice::invertSomeClusters() {
    std::vector<std::vector<std::vector<bool>>> visited = std::vector<std::vector<std::vector<bool>>>(length, std::vector<std::vector<bool>>(length, std::vector<bool>(length, false)));
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            for (int k = 0; k < length; k++) {
                if (visited[i][j][k]) continue;
                bool invertThisCluster = randomZeroOne->get();
                std::queue<std::tuple<int, int, int>> q;
                q.push(std::make_tuple(i, j, k));
                visited[i][j][k] = true;
                if (invertThisCluster) invertSpin(i, j, k);
                while (!q.empty()) {
                    std::tuple<int, int, int> front = q.front();
                    q.pop();
                    for (std::tuple<int, int, int> neighbor : bonds[std::get<0>(front)][std::get<1>(front)][std::get<2>(front)]) {
                        if (!visited[std::get<0>(neighbor)][std::get<1>(neighbor)][std::get<2>(neighbor)]) {
                            visited[std::get<0>(neighbor)][std::get<1>(neighbor)][std::get<2>(neighbor)] = true;
                            q.push(neighbor);
                            if (invertThisCluster) invertSpin(std::get<0>(neighbor), std::get<1>(neighbor), std::get<2>(neighbor));
                        }
                    }
                }
            }
        }
    }
}
