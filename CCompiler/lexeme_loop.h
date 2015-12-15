#pragma once
#include "context.h"
#include "lexeme_interface.h"

/*
	Holds the for/while loop lexeme
*/
class LexemeLoop : public LexemeInterface
{
public:
	LexemeLoop(const LexemeType& type, const std::string& value) : LexemeLoop(type, value, 0) {}
	LexemeLoop(const LexemeType& type, const std::string& value, int lvl) :LexemeInterface(type, value, lvl), condition_(nullptr), body_(nullptr) {}

	virtual ~LexemeLoop() = default;

	auto condition() const { return condition_; }
	auto body() const { return body_; }

	void set_condition(std::shared_ptr<Context> cond) { condition_ = cond; }
	void set_body(std::shared_ptr<Context> body) { body_ = body; }
private:
	std::shared_ptr<Context> condition_; /*!< for loop holds 3 statements here, while - only 1*/
	std::shared_ptr<Context> body_; /*!< curly braced block of statements here*/
};
