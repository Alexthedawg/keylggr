CC=gcc
CXX=g++
CFLAGS=-Wall -Werror -g

TARGETS= keylggr

All: $(TARGETS)

keylggr: keylggr.o
	$(CXX) $(CFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGETS)
