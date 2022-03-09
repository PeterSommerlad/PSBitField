SRC=src/PSBitFieldTest.cpp

all: src/PSBitFieldTest.cpp psbitfield.h
	g++ -std=c++17 -I. -I./cute  -o PSBitFieldTest -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wno-attributes src/PSBitFieldTest.cpp
	
check: all
	./PSBitFieldTest