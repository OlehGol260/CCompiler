#pragma once
#include "enums.h"

class LexemInterface
{
public:
	LexemInterface(const LexemType& type, const std::string& value) : LexemInterface(type, value, 0) {}
	LexemInterface(const LexemType& type, const std::string& value, int level) : type_(type), value_(value), level_(level){}

	virtual ~LexemInterface() = default;

	auto type() const { return type_; }
	auto value() const { return value_; }
	auto level() const noexcept { return level_; }

	void set_type(LexemType type) { type_ = type; }
	void set_value(const std::string& code) { value_ = code; }
	void set_level(int level) { level_ = level; }
private:
	LexemType type_;
	std::string value_;
	int level_;
};
