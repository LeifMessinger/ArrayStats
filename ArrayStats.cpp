#include "ArrayStats.h"

//This is the most useless source file because all the templated functions I can't do here.

void ArrayStats::IncreasingDecreasing::print(){
        std::cout << "Increasing: " << increasing << std::endl;
        std::cout << "Strictly increasing: " << strictlyIncreasing << std::endl;
        std::cout << "Decreasing: " << decreasing << std::endl;
        std::cout << "Strictly decreasing: " << strictlyDecreasing << std::endl;
}
