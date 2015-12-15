#pragma once
#include <memory>

#include "lexeme_interface.h"

/*!
	Contains if/else statement
	If 'if' doesn't have 'else' statement, else_body_ will be nullptr
*/
class LexemeCondition : public LexemeInterface
{
public:
	LexemeCondition(const LexemeType& type, const std::string& value) : LexemeCondition(type, value, 0) {}
	LexemeCondition(const LexemeType& type, const std::string& value, int lvl) :LexemeInterface(type, value, lvl), condition_(nullptr), if_body_(nullptr), else_body_(nullptr) {}

	virtual ~LexemeCondition() = default;

	auto conidition() const { return condition_; }
	auto if_body() const { return if_body_; }
	auto else_body() const { return else_body_; }

	void set_condition(std::shared_ptr<Statement> cond) { condition_ = cond; }
	void set_if_body(std::shared_ptr<Context> if_body) { if_body_ = if_body; }
	void set_else_body(std::shared_ptr<Context> else_body) { else_body_ = else_body; }
private:
	std::shared_ptr<Statement> condition_; /*!< logical condition. If evaluates to true, if_body_ will be executed, else_body_ otherwise */
	std::shared_ptr<Context> if_body_; /*!< curly braced block which will be executed if condition_ is true*/
	std::shared_ptr<Context> else_body_; /*!< curly braced block which will be executed if condition_ is false*/

};
