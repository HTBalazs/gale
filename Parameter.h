#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>

template <typename T>
class Parameter_base {
private:
	std::string name_;
	T min_;
	T max_;
public:
	Parameter_base() = delete;
	Parameter_base(std::string const& name, T const& min, T const& max) {
		name_ = name;
		min_ = min;
		max_ = max;
	}
	std::string get_name() const {
		return name_;
	}
	T get_min() const {
		return min_;
	}
	T get_max() const {
		return max_;
	}
};

class Generic_parameter {
protected:
	virtual std::shared_ptr<Generic_parameter> cross_impl(Generic_parameter const& other) =0;
public:
	virtual ~Generic_parameter() {}
	virtual std::string get_name() const=0;
	virtual void print(std::ostream& os) const=0;
	std::shared_ptr<Generic_parameter> cross(Generic_parameter const& other);
};

template <typename T>
class Parameter : public Generic_parameter {
private:
	std::shared_ptr<Parameter_base<T>> base_;
	T value_;
	std::shared_ptr<Generic_parameter> cross_impl(Generic_parameter const& other) override {
		return std::make_shared<Parameter<T>>();
	}
public:
	~Parameter() {}
	Parameter(std::shared_ptr<Parameter_base<T>> b, T const& v=T{}) : base_{b}, value_{v} {
		if(value_ < base_->get_min()) { value_ = base_->get_min(); }
		if(value_ > base_->get_max()) { value_ = base_->get_max(); }
	}
	std::shared_ptr<Parameter_base<T>> get_base() const {
		return base_;
	}
	void set_value(T const& v) {
		value_ = v;
	}
	T get_value() const {
		return value_;
	}
	std::string get_name() const override {
		return base_->get_name();
	}
	void print(std::ostream& os) const override {
		os << *base_ << std::endl;
		os << "    Value:   " << value_ << std::endl;
	}
	std::shared_ptr<Parameter<T>> cross(Parameter<T> const& other) {
		return std::static_pointer_cast<Parameter<T>, Generic_parameter>(cross_impl(other));
	}
};
// new Parameter<T>{base_, (this->value+other.value)/2.0}

template <typename T>
std::ostream& operator<<(std::ostream& os, Parameter_base<T> const& p) {
	os << "    Name:    " << p.get_name() << std::endl;
	os << "    Minimum: " << p.get_min() << std::endl;
	os << "    Maximum: " << p.get_max();
	return os;
}
