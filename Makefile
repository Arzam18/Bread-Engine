CXX ?= g++
CXXFLAGS ?= -O3 -std=c++20 -Wall -Wextra

bread_SRC := .

native: ARCH := native
avx2:   ARCH := x86-64-v3
avx512: ARCH := x86-64-v4
arm64:  ARCH := armv8-a

native avx2 avx512 arm64: bread_engine

bread_engine: nnue.cpp
	$(CXX) $(CXXFLAGS) -march=$(ARCH) -I$(bread_SRC) $< -o $@

clean:
	rm -f bread_engine

.PHONY: native avx2 avx512 arm64 clean
