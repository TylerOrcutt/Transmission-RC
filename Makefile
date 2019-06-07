ifeq ($(PREFIX),)
	PREFIX := /usr
endif

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

LDFLAGS = -lcurl -lncurses -pthread

transmission-rc: $(obj)
	$(CXX) -std=c++17 -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(obj) transmission-rc

install: transmission-rc
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/transmission-rc
.PHONY: uninstall
uninstall: 
	rm -f $(DESTDIR)$(PREFIX)/bin/transmission-rc
