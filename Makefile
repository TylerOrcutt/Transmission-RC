all: *.cpp
	g++ *.cpp -lcurl -lncurses -o TransmissionRC
