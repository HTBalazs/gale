#pragma once

#include <vector>
#include <string>
#include "Parameter.h"

#define random(min, max) (((double)(rand()%1000)/1000.0f)*((max)-(min))+(min))

class Element final {
	std::vector<std::shared_ptr<Generic_parameter>> parameter_;
	double fitness_ = 0.0;
public:
	Element() {}
	Element(Element const& e1, Element const& e2) {
		size_t size = e1.parameter_.size();
		for(int i=0; i<size; i++) {
			this->parameter_.push_back(e1.parameter_[0]->cross(*e2.parameter_[0]));
		}
	}
	template <typename T> T get(std::string s) const {
		for(auto it : parameter_) {
			std::shared_ptr<Parameter<T>> p = std::dynamic_pointer_cast<Parameter<T>>(it);
			if(p->get_name() == s) {
				return p->get_value();
			}
		}
		std::cout << "No such parameter: " << s << std::endl;
		return T{};
	}
	void add_parameter(Generic_parameter* param) {
		parameter_.push_back(std::shared_ptr<Generic_parameter>{param});
	}
	std::shared_ptr<Generic_parameter> get_parameter(std::string const& name) {
		for(auto const& it:parameter_) {
			if(it->get_name()==name) {
				return it;
			}
		}	
		throw false;
	}
	friend std::ostream& operator<<(std::ostream& os, Element const& e);
};

std::ostream& operator<<(std::ostream& os, Element const& e) {
	for(auto it:e.parameter_) {
		it->print(os);
	}
	os << "    Fitness:  " << e.fitness_ << std::endl; 
	return os;
}

Element operator+(Element const& parent1, Element const& parent2) {
	Element child{parent1, parent2};
	return child;
}


