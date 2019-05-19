all: *.cpp
	g++ *.cpp -lcurl -lncurses -pthread -std=c++17 -o TransmissionRC
