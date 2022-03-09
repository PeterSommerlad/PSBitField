SRC=src/PSBitFieldTest.cpp

all : ./PSBitFieldTest

./PSBitFieldTest: src/PSBitFieldTest.cpp psbitfield.h
	g++ -std=c++17 -I. -I./cute  -o PSBitFieldTest -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wno-attributes src/PSBitFieldTest.cpp
	
check: ./PSBitFieldTest
	./PSBitFieldTest
	
clean: 
	rm ./PSBitFieldTest ./PSBitFieldTest.xml