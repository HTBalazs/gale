#include <iostream>
#include <cmath>
#include "Problem.h"

// Compile:
// g++ -std=c++11 GA_solver.cpp -o GA_solver.exe

/* Example: 
	Given the bending torque T and the maximum allowed stress "S" of a beam.
	Find the best rectangular beam cross-section with the smallest area using the width and height parameters.
*/

using mytype = double;

int main() {

	Problem<mytype> p;
	p.addParameter("x", 0, 1);
	p.addParameter("y", 0, 1);

	auto fintessfunction = [](Element<mytype> const& e) {
		return 1.0/pow(e["x"] + e["y"]-0.6,2);
	};

	p.solve(fintessfunction, 100, 10);	// (fitnessfnc, number of individuals, number of generations)

	std::cout << p << std::endl;

	return 0;
}