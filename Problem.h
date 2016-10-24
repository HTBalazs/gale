#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "Parameter.h"
#include "Element.h"

#define abs(a) (((a)<0)?(-a):(a))

template <typename T>
class Problem final {
private:
	// using processptr = double(*)(Element<T> const&);
	using processptr = std::function<double(Element<T> const&)>;
	void initializeElements(int numElements);
	double calculateFitnessSum() const;
	Element<T>* selectParent(int);
	void selection();
	void crossover();
	void mutation();
	void checkLimits();
	double calculateFitnessValues(processptr);
	double calculateThreshold() const;
public:
	std::vector<Parameter<T>> parameter;
	std::vector<Element<T>> element;
	int numel;
	double threshold;

	Problem() {}
	Element<T> getBestOne() const;
	void addParameter(std::string, T, T);
	void solve(processptr, int, int);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Problem<T> const& p) {
	os << "\nProblem:" << std::endl;
	os << "\n  Parameters:" << std::endl;
	for(auto it:p.parameter)
		os << it << std::endl;
	os << "  Best element:\n" << p.getBestOne() << std::endl;

	return os;
}

template <typename T>
void Problem<T>::initializeElements(int numElements) {
	element.reserve(numElements);
	for(int i=0; i<numElements; i++)
		element.push_back(Element<T>{parameter});
}

template <typename T>
double Problem<T>::calculateFitnessSum() const {
	double sum = 0;
	for(auto it:element) {
		sum += it.fitness_;
	}
	return sum;
}

template <typename T>
Element<T>* Problem<T>::selectParent(int S) {
	double r = random(0,S);
	double s = 0;
	for(auto& it:element) {
		s += it.fitness_;
		if(s>r) {
			return &it;
		}
	}
}

template <typename T>
void Problem<T>::selection() {
	int i=0;
	while(i<element.size()) {
		if(element[i].fitness_<threshold){
			element.erase(element.begin()+i);
		} else {
			i++;
		}
	}
}

template <typename T>
void Problem<T>::crossover() {
	double S = calculateFitnessSum();
	while(element.size()<numel) {
		Element<T>* p1;
		Element<T>* p2;
		int i = 0;
		int maxit = 10;
		do {
			p1 = selectParent(S);
			p2 = selectParent(S);
			i++;
		} while(p1==p2 && i<maxit);
		element.push_back(*p1+*p2);
	}
}

template <typename T>
void Problem<T>::mutation() {
	for(auto& it : element) {
		for(auto& jt : it.p_) {
			jt.value_ += random(-0.01*abs(jt.value_), 0.01*abs(jt.value_));
		}
	}
}
template <typename T>
void Problem<T>::checkLimits() {
	for(auto& it : element) {
		for(int i=0; i<it.p_.size(); i++) {
			if(it.p_[i].value_ < parameter[i].min_)
				it.p_[i].value_ = parameter[i].min_;
			if(it.p_[i].value_ > parameter[i].max_)
				it.p_[i].value_ = parameter[i].max_;
		}
	}
}

template <typename T>
double Problem<T>::calculateFitnessValues(processptr process) {
	// calculate fitness values
	for(auto& it:element)
		it.fitness_ = process(it);
	return calculateThreshold();
}

template <typename T>
double Problem<T>::calculateThreshold() const {
	return calculateFitnessSum() / element.size();
}

template <typename T>
Element<T> Problem<T>::getBestOne() const {
	int maxfitness = 0;
	int idx = 0;
	for(int i=0; i<element.size(); i++){
		if(maxfitness<element[i].fitness_) {
			maxfitness = element[i].fitness_;
			idx = i;
		}
	}
	return element[idx];
}

template <typename T>
void Problem<T>::addParameter(std::string name, T min, T max) {
	parameter.push_back(Parameter<T>{name, min, max});
}

template <typename T>
void Problem<T>::solve(processptr process, int numElements, int numGenerations) {
	numel = numElements;
	initializeElements(numElements);

	threshold = calculateFitnessValues(process);

	for(int i=0; i<numGenerations; i++) {
		selection();
		crossover();
		mutation();
		checkLimits();

		threshold = calculateFitnessValues(process);
	}
}