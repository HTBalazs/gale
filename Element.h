#pragma once
#include <vector>
#include <string>
#include "Parameter.h"

#define random(min, max) (((float)(rand()%1000)/1000.0f)*((max)-(min))+(min))

template <typename T>
class Element final {
	struct Param {
		std::string name_;
		T value_;
		Param() = default;
		Param(std::string n, T v) : name_{n}, value_{v} {}
	};
public:
	double fitness_;
	std::vector<Param> p_;
	Element() = delete;
	Element(int size);
	Element(std::vector<Parameter<T>> const&);
	Element(Element<T> const&);
	Element(Element<T>, Element<T>);
	T operator[](std::string) const;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Element<T> const& e) {
	for(auto it:e.p_)
		os << "      " << it.name_ << ": " << it.value_ << std::endl;
	os << "      Fitness:  " << e.fitness_ << std::endl; 
	return os;
}

template <typename T>
Element<T> operator+(Element<T> const& parent1, Element<T> const& parent2) {
	Element<T> child{parent1, parent2};
	return child;
}

template <typename T>
T Element<T>::operator[](std::string s) const {
	for(auto it : p_) {
		if(it.name_ == s) {
			return it.value_;
		}
	}
	std::cout << "No such parameter: " << s << std::endl;
	return (T)0;
}

template <typename T>
Element<T>::Element(int size) {
	p_.reserve(size);
	for(int i=0; i<size; i++) {
		p_.push_back(Param{});
	}
}

template <typename T>
Element<T>::Element(std::vector<Parameter<T>> const& p) {
	fitness_ = 0;
	p_.reserve(p.size());
	for(int i=0; i<p.size(); i++)
		p_.push_back(Param{p[i].name_, T{random(p[i].min_, p[i].max_)}});
}

template <typename T>
Element<T>::Element(Element<T> const& other) {
	this->fitness_ = other.fitness_;
	this->p_ = other.p_;
}

template <typename T>
Element<T>::Element(Element<T> p1, Element<T> p2) {
	int size = p1.p_.size();
	fitness_ = 0;

	for(int i=0; i<size; i++) {
		double weight = random(0,1);
		double value = weight*p1.p_[i].value_ + (1.0-weight)*p2.p_[i].value_;
		p_.push_back(Param{p1.p_[i].name_, T{value}});
	}
}