#pragma once

#include <memory>

#include "enums.h"
#include "lexeme_interface.h"
class Lexeme : public LexemeInterface
{
public:
	Lexeme(const LexemeType& type, const std::string& value) : Lexeme(type, value, 0) {}
	Lexeme(const LexemeType& type, const std::string& value, int state) :LexemeInterface(type, value, state), left_lexem_(nullptr), right_lexem_(nullptr) {}
	
	virtual ~Lexeme() = default;

	auto left() const { return left_lexem_; }
	auto right() const { return right_lexem_; }

	void set_left(std::shared_ptr<LexemeInterface> left) { left_lexem_ = left; }
	void set_right(std::shared_ptr<LexemeInterface> right) { right_lexem_ = right; }
private:
	std::shared_ptr<LexemeInterface> left_lexem_;
	std::shared_ptr<LexemeInterface> right_lexem_;
};
