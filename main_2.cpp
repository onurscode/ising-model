#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include "random.h"
#include "lattice_2.h"

std::pair<double, double> simulateForBetaJ(int length, int warmupCount, int iterationCount, double betaJ, RandomZeroOne *R01Generator, RandomBetweenZeroOne *RB01Generator) {

    Lattice lattice = Lattice(length, R01Generator, RB01Generator);
    lattice.setBetaJ(betaJ);

    for (int i = 0; i < warmupCount; i++) {
        lattice.formSomeBonds();
        lattice.invertSomeClusters();
        lattice.breakAllBonds();
    }

    long long absoluteSpinSumSum = 0;
    long long spinSumSquaredSum = 0;

    for (int i = 0; i < iterationCount - warmupCount; i++) {
        lattice.formSomeBonds();
        lattice.invertSomeClusters();

        int latticeSpinSum = lattice.getSpinSum();
        absoluteSpinSumSum += (latticeSpinSum < 0) ? -latticeSpinSum : latticeSpinSum;
        spinSumSquaredSum += latticeSpinSum * latticeSpinSum;

        lattice.breakAllBonds();
    }

    double magnetization = (double) absoluteSpinSumSum / (iterationCount - warmupCount);
    double susceptibility = ((double) spinSumSquaredSum / (iterationCount - warmupCount)) - (magnetization * magnetization);

    return std::make_pair(magnetization, susceptibility);
}

void simulate(int length, int numberOfDataPoints, double tStart, double tEnd, int warmupCount, int iterationCount, RandomZeroOne *R01Generator, RandomBetweenZeroOne *RB01Generator, std::ofstream &fileM, std::ofstream &fileS) {

    double tIncrement = (tEnd - tStart) / numberOfDataPoints;

    double t = tStart;
    for (int i = 0; i < numberOfDataPoints; i++) {
        double betaJ = 1 / t;
        std::pair<double, double> result = simulateForBetaJ(length, warmupCount, iterationCount, betaJ, R01Generator, RB01Generator);

        double magnetization = result.first;
        double susceptibility = result.second;

        fileM << t << " " << magnetization / (length * length) << "\n";
        fileS << t << " " << susceptibility << "\n";

        std::cout << "\033[1F\033[10C\033[0K" << i + 1 << " / " << numberOfDataPoints << "\n"; // Move cursor to the start of previous line, move cursor 10 cells right, clear the rest of the line, output the progress. See: https://en.wikipedia.org/wiki/ANSI_escape_code

        t += tIncrement;
    }
}

int main() {

    int length, numberOfDataPoints, warmupCount, iterationCount, seed;
    double betaJStart, betaJEnd;
    std::string fileNameM, fileNameS, seedAnswer;
    bool wantCustomSeed = false;

    std::cout << "Enter a length for lattice: ";
    std::cin >> length;
    std::cout << "Enter the number of data points to generate per plot: ";
    std::cin >> numberOfDataPoints;
    std::cout << "Enter the value beta * J should start at: ";
    std::cin >> betaJStart;
    std::cout << "Enter the value beta * J should end at: ";
    std::cin >> betaJEnd;
    std::cout << "Enter warmup iteration count: ";
    std::cin >> warmupCount;
    std::cout << "Enter total iteration count (including warmup): ";
    std::cin >> iterationCount;
    std::cout << "Enter a file name or file path for magnetization data: ";
    std::cin >> fileNameM;
    std::cout << "Enter a file name or file path for susceptibility data: ";
    std::cin >> fileNameS;
    std::cout << "Do you want to enter a custom seed for random? (yes/no) ";
    std::cin >> seedAnswer;
    if (seedAnswer == "yes") {
        wantCustomSeed = true;
        std::cout << "Enter seed: ";
        std::cin >> seed;
    }


    RandomZeroOne *randomZeroOne;
    RandomBetweenZeroOne *randomBetweenZeroOne;
    if (wantCustomSeed) {
        randomZeroOne = new RandomZeroOne(seed);
        randomBetweenZeroOne = new RandomBetweenZeroOne(seed);
    } else {
        randomZeroOne = new RandomZeroOne();
        randomBetweenZeroOne = new RandomBetweenZeroOne();
    }

    std::ofstream fileM(fileNameM);
    std::ofstream fileS(fileNameS);


    double tStart = 1 / betaJEnd;
    double tEnd = 1 / betaJStart;

    std::cout << "\n\n\n\nProgress: 0 / " << numberOfDataPoints << "\n";
    simulate(length, numberOfDataPoints, tStart, tEnd, warmupCount, iterationCount, randomZeroOne, randomBetweenZeroOne, fileM, fileS);


    delete randomZeroOne;
    delete randomBetweenZeroOne;
    fileM.close();
    fileS.close();

    return 0;
}
