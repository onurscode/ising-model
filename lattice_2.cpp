#include "lattice_2.h"
#include <cmath>
#include <queue>

Lattice::Lattice(int length, RandomZeroOne *randomZeroOne, RandomBetweenZeroOne *randomBetweenZeroOne) :
        lattice(std::vector<std::vector<int>>(length, std::vector<int>(length, 1))),
        bonds(std::vector<std::vector<std::vector<std::pair<int, int>>>>(length, std::vector<std::vector<std::pair<int, int>>>(length, std::vector<std::pair<int, int>>()))),
        length(length),
        size(length * length) {
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
    bonds = std::vector<std::vector<std::vector<std::pair<int, int>>>>(length, std::vector<std::vector<std::pair<int, int>>>(length, std::vector<std::pair<int, int>>()));
    thereMayBeBonds = false;
}

class ThereMayBeBondsException {};

void Lattice::formSomeBonds() {
    if (!betaJIsSet) throw BetaJIsNotSetException();
    if (thereMayBeBonds) throw ThereMayBeBondsException();
    thereMayBeBonds = true;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            int i_next = (i == length - 1) ? 0 : i + 1;
            int j_next = (j == length - 1) ? 0 : j + 1;
            if (lattice[i][j] == lattice[i_next][j]) {
                if (randomBetweenZeroOne->get() < bondingProbability) {
                    bonds[i][j].push_back(std::make_pair(i_next, j));
                    bonds[i_next][j].push_back(std::make_pair(i, j));
                }
            }
            if (lattice[i][j] == lattice[i][j_next]) {
                if (randomBetweenZeroOne->get() < bondingProbability) {
                    bonds[i][j].push_back(std::make_pair(i, j_next));
                    bonds[i][j_next].push_back(std::make_pair(i, j));
                }
            }
        }
    }
}

void Lattice::invertSpin(int i, int j) {
    if (lattice[i][j] == 1) {
        lattice[i][j] = -1;
        spinSum -= 2;
    } else {
        lattice[i][j] = 1;
        spinSum += 2;
    }
}

void Lattice::invertSomeClusters() {
    std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>(length, std::vector<bool>(length, false));
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (visited[i][j]) continue;
            bool invertThisCluster = randomZeroOne->get();
            std::queue<std::pair<int, int>> q;
            q.push(std::make_pair(i, j));
            visited[i][j] = true;
            if (invertThisCluster) invertSpin(i, j);
            while (!q.empty()) {
                std::pair<int, int> front = q.front();
                q.pop();
                for (std::pair<int, int> neighbor : bonds[front.first][front.second]) {
                    if (!visited[neighbor.first][neighbor.second]) {
                        visited[neighbor.first][neighbor.second] = true;
                        q.push(neighbor);
                        if (invertThisCluster) invertSpin(neighbor.first, neighbor.second);
                    }
                }
            }
        }
    }
}
