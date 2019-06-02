all:
	g++ -lcurl `pkg-config gtk+-3.0 -cflags` `pkg-config gtkmm-3.0 -cflags` *.cpp ../TransmissionRPCRequest.cpp ../TransmissionRC.cpp ../config.cpp ../Utility.cpp  -o TransmissionRC-gtk `pkg-config gtk+-3.0 --libs` `pkg-config gtkmm-3.0 --libs`
