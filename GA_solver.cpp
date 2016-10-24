#include <iostream>
#include <cmath>
#include "Problem.h"

// Compile:
// g++ -std=c++11 GA_solver.cpp -o GA_solver.exe

/* Example: 
	Given the bending torque T and the maximum allowed stress "Smax" of a beam.
	Find the best rectangular beam cross-section with the smallest area using the width and height parameters.
*/

using mytype = double;

int main() {

	double const Smax = 200e6;
	double const T = 1e4;
	Problem<mytype> p;
	p.addParameter("width", 0.001, 1);
	p.addParameter("height", 0.001, 1);

	auto fintessfunction = [&Smax,&T](Element<mytype> const& e) {
		double S = 6*T/e["height"]/e["height"]/e["width"];
		if(S>Smax) {
			return 0.0;
		} else {
			return 1.0/(e["height"]*e["width"]);
		}
	};

	p.solve(fintessfunction, 1000, 100);	// (fitnessfnc, number of individuals, number of generations)

	std::cout << p << std::endl;

	return 0;
}