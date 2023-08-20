/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include "ArrayStats.h"
#include <vector>

int main()
{
	const std::vector<int> list1 = {3, 8, 5, 1, 2, 7, 3, 4};

	ArrayStats::getStats(list1.begin(), list1.end()).print();

	std::cout << std::endl;

	const std::vector<int> list2 = {4, 3, 1, 7, 3, 8};

	ArrayStats::getStats(list2.begin(), list2.end()).print();

	return 0;
}
