#pragma once
#include "enums.h"

class LexemeInterface
{
public:
	LexemeInterface(const LexemeType& type, const std::string& value) : LexemeInterface(type, value, 0) {}
	LexemeInterface(const LexemeType& type, const std::string& value, int level) : type_(type), value_(value), level_(level){}

	virtual ~LexemeInterface() = default;

	auto type() const { return type_; }
	auto value() const { return value_; }
	auto level() const noexcept { return level_; }

	void set_type(LexemeType type) { type_ = type; }
	void set_value(const std::string& code) { value_ = code; }
	void set_level(int level) { level_ = level; }
private:
	LexemeType type_;
	std::string value_;
	int level_;
};
