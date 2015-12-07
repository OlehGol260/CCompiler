#pragma once
#include "lexeme_interface.h"
#include <memory>

class LexemePrint : public LexemeInterface
{
public:
	LexemePrint(const LexemeType& type, const std::string& value) : LexemePrint(type, value, 0) {}
	LexemePrint(const LexemeType& type, const std::string& value, int lvl) :LexemeInterface(type, value, lvl), body_(nullptr) {}

	virtual ~LexemePrint() = default;

	auto body() const { return body_; }

	void set_body(std::shared_ptr<LexemeInterface> body) { body_ = body; }
private:
	std::shared_ptr<LexemeInterface> body_;
};
