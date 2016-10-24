#pragma once
#include <vector>
#include <string>

template <typename T>
class Parameter final {
public:
	std::string name_;
	T min_;
	T max_;
	Parameter() = delete;
	Parameter(std::string name, T min, T max) : name_{name}, min_{min}, max_{max} {}
};

template<typename T>
std::ostream& operator<<(std::ostream& os, Parameter<T> const& p) {
	os << "    Name:    " << p.name_ << std::endl;
	os << "    Minimum: " << p.min_ << std::endl;
	os << "    Maximum: " << p.max_ << std::endl;
	return os;
}