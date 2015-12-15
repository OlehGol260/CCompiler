#pragma once
#include "lexeme_interface.h"
#include <memory>

/*
	Holds info about sqrt and print lexemes
*/
class LexemeFunc : public LexemeInterface
{
public:
	LexemeFunc(const LexemeType& type, const std::string& value) : LexemeFunc(type, value, 0) {}
	LexemeFunc(const LexemeType& type, const std::string& value, int lvl) :LexemeInterface(type, value, lvl), body_(nullptr) {}

	virtual ~LexemeFunc() = default;

	auto body() const { return body_; }

	void set_body(std::shared_ptr<LexemeInterface> body) { body_ = body; }
private:
	std::shared_ptr<LexemeInterface> body_; /*!< body of the function*/
};
