#pragma once

#include <string>
#include <memory>

#include "enums.h"

class Lexem
{
public:
	Lexem(const LexemType& type, const std::string& value) : Lexem(type, value, 0) {}
	Lexem(const LexemType& type, const std::string& value, int state) : type_(type), value_(value), state_(state), left_lexem_(nullptr), right_lexem_(nullptr) {}
	//explicit Lexem(int state) : type_(LexemType::kNull), value_(""), state_(state), left_lexem_(nullptr), right_lexem_(nullptr) {}

	~Lexem() = default;

	//bool IsTerminal() const noexcept { return type_ == LexemType::kTerminal; }
	//bool IsNonTerminal() const noexcept { return type_ == LexemType::kNonTerminal; }

	auto type() const { return type_; }
	auto value() const { return value_; }
	auto state() const noexcept { return state_; }

	void set_type(LexemType type) { type_ = type; }
	void set_value(const std::string& code) { value_ = code; }
	void set_state(int state) { state_ = state; }


	auto left() const { return left_lexem_; }
	auto right() const { return right_lexem_; }

	void set_left(std::shared_ptr<Lexem> left) { left_lexem_ = left; }
	void set_right(std::shared_ptr<Lexem> right) { right_lexem_ = right; }
private:
	LexemType type_;
	std::string value_;
	int state_;

	//for non-terminal this both value will be nullptr
	std::shared_ptr<Lexem> left_lexem_;
	std::shared_ptr<Lexem> right_lexem_;
};
