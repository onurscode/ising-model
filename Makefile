all: main_2 main_3

main_2:
	g++ -std=c++17 main_2.cpp random.cpp lattice_2.cpp -o main_2

main_3:
	g++ -std=c++17 main_3.cpp random.cpp lattice_3.cpp -o main_3
