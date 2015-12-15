#pragma once
#include "enums.h"

/* 
	Holds common info about a lexeme - its type, string value and level
*/
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
	LexemeType type_; /*!< type of the lexeme */
	std::string value_; /*!< name of the underlying element*/
	int level_; /*!< Used in parentheses precedence. Has no effect in curly braced block*/
};
