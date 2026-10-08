CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic

.PHONY: all test benchmark

all:
	$(CXX) $(CXXFLAGS) -fsyntax-only -x c++ unique_pointer.hpp
	$(CXX) $(CXXFLAGS) -fsyntax-only -x c++ shared_pointer.hpp

test:
	$(CXX) $(CXXFLAGS) -pthread -I googletest/googletest/include -I googletest/googletest smart_pointer_tests.cpp googletest/googletest/src/gtest-all.cc googletest/googletest/src/gtest_main.cc -o smart_pointer_tests
	./smart_pointer_tests

benchmark:
	$(CXX) $(CXXFLAGS) -O2 main.cpp -o benchmark
	./benchmark

debug: CXXFLAGS += -g -o0
debug: main