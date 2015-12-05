#pragma once

#include <memory>

#include "enums.h"
#include "lexem_interface.h"
class Lexem : public LexemInterface
{
public:
	Lexem(const LexemType& type, const std::string& value) : Lexem(type, value, 0) {}
	Lexem(const LexemType& type, const std::string& value, int state) :LexemInterface(type, value, state), left_lexem_(nullptr), right_lexem_(nullptr) {}
	
	virtual ~Lexem() = default;

	auto left() const { return left_lexem_; }
	auto right() const { return right_lexem_; }

	void set_left(std::shared_ptr<LexemInterface> left) { left_lexem_ = left; }
	void set_right(std::shared_ptr<LexemInterface> right) { right_lexem_ = right; }
private:
	std::shared_ptr<LexemInterface> left_lexem_;
	std::shared_ptr<LexemInterface> right_lexem_;
};
