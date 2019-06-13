#all:
#g++ -lcurl `pkg-config gtk+-3.0 -cflags` `pkg-config gtkmm-3.0 -cflags` *.cpp ../TransmissionRPCRequest.cpp ../TransmissionRC.cpp ../config.cpp ../Utility.cpp ../args.cpp -o TransmissionRC-gtk `pkg-config gtk+-3.0 --libs` `pkg-config gtkmm-3.0 --libs`
ifeq ($(PREFIX),)
	PREFIX := /usr
endif

CXX ?= g++
PROGRAM = transmission-rc-gtk
CXXFILES = $(wildcard *.cpp) ../TransmissionRPCRequest.cpp ../TransmissionRC.cpp ../config.cpp ../Utility.cpp ../args.cpp 

OBJS = $(CXXFILES:.cpp=.o)
DEPDIR = .deps
CXXFLAGS = `pkg-config --cflags gtkmm-3.0` -std=c++17 
LIBS = `pkg-config --libs gtkmm-3.0` -lcurl -pthread

all: $(PROGRAM)

-include $(OBJS:%.o=$(DEPDIR)/%.Po)

%.o: %.cpp
	$(CXX) -MT $@ -MD -MP -MF $*.o -c -o $@ $(CXXFLAGS) $<

$(PROGRAM): $(OBJS)
	$(CXX) -o $(PROGRAM) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)

install: $(PROGRAM)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/$(PROGRAM)
