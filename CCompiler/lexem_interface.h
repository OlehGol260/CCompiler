#pragma once
#include "enums.h"

class LexemInterface
{
public:
	LexemInterface(const LexemType& type, const std::string& value) : LexemInterface(type, value, 0) {}
	LexemInterface(const LexemType& type, const std::string& value, int state) : type_(type), value_(value), state_(state){}

	virtual ~LexemInterface() = default;

	auto type() const { return type_; }
	auto value() const { return value_; }
	auto state() const noexcept { return state_; }

	void set_type(LexemType type) { type_ = type; }
	void set_value(const std::string& code) { value_ = code; }
	void set_state(int state) { state_ = state; }
private:
	LexemType type_;
	std::string value_;
	int state_;
};
