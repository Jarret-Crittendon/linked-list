all: ./src/main.cpp
	clang++ -std=c++20 -Wall -Werror -pedantic ./src/main.cpp -o ./doublelist.exe -Os