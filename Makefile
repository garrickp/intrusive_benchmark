CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Wno-c++11-extensions -O2

all: benchmark_v benchmark_ill

benchmark_v: benchmark_v.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

benchmark_ill: benchmark_ill.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm benchmark_v benchmark_ill
