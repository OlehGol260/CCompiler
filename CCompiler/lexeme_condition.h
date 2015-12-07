#pragma once
#include "lexeme_interface.h"
#include <memory>

class LexemeCondition : public LexemeInterface
{
public:
	LexemeCondition(const LexemeType& type, const std::string& value) : LexemeCondition(type, value, 0) {}
	LexemeCondition(const LexemeType& type, const std::string& value, int lvl) :LexemeInterface(type, value, lvl), condition_(nullptr), if_body_(nullptr), else_body_(nullptr) {}

	virtual ~LexemeCondition() = default;

	auto conidition() const { return condition_; }
	auto if_body() const { return if_body_; }
	auto else_body() const { return else_body_; }

	void set_condition(std::shared_ptr<LexemeInterface> cond) { condition_ = cond; }
	void set_if_body(std::shared_ptr<LexemeInterface> if_body) { if_body_ = if_body; }
	void set_else_body(std::shared_ptr<LexemeInterface> else_body) { else_body_ = else_body; }
private:
	std::shared_ptr<LexemeInterface> condition_;
	std::shared_ptr<LexemeInterface> if_body_;
	std::shared_ptr<LexemeInterface> else_body_;

};
