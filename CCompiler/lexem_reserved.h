#pragma once
#include "lexem_interface.h"
#include "context.h"

class LexemReserved : public LexemInterface
{
public:
	LexemReserved(const LexemType& type, const std::string& value) : LexemReserved(type, value, 0) {}
	LexemReserved(const LexemType& type, const std::string& value, int lvl) :LexemInterface(type, value, lvl), condition_(nullptr), body_(nullptr) {}

	virtual ~LexemReserved() = default;

	auto condition() const { return condition_; }
	auto body() const { return body_; }

	void set_condition(std::shared_ptr<Context> cond) { condition_ = cond; }
	void set_body(std::shared_ptr<Context> body) { body_ = body; }
private:
	std::shared_ptr<Context> condition_;
	std::shared_ptr<Context> body_;
};
