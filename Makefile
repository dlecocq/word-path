CPP = g++
CPPOPTS = -O3 -Wall -g

all: driver

driver: driver.cpp src/dictionary.hpp src/rules/standard.hpp
	$(CPP) $(CPPOPTS) -o driver driver.cpp -Isrc/

clean:
	rm -rdf *.o *.dSYM driver
