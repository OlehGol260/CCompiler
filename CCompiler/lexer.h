#pragma once
#include <string>
#include <vector>
#include "lexeme.h"

class Lexer
{
public:
	void Parse(const std::string& code_);
	static std::string AddSpaces(const std::string& code_);
	auto lexems() const noexcept { return lexems_; }
	void print() const;
private:
	std::vector<std::shared_ptr<LexemeInterface>> lexems_;
};
