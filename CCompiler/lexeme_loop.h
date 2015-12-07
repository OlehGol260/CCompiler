#pragma once
#include "context.h"
#include "lexeme_interface.h"

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
	std::shared_ptr<Context> condition_;
	std::shared_ptr<Context> body_;
};
