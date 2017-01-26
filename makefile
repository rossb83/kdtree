CXX=g++
CXXFLAGS=-std=c++1y

./%.o: %.c
	$(CXX) -c -o $@ $< $(CXXFLAGS)

all: PointTest RunTests build_kdtree query_kdtree

PointTest: PointTest.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

build_kdtree: build_kdtree.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

query_kdtree: query_kdtree.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

RunTests:
	./PointTest

.PHONY: clean

clean:
	rm -f ./*.o *~ core
 
