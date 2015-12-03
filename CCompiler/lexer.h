#pragma once
#include <string>
#include <vector>
#include "lexem.h"

class Lexer
{
public:
	void GenerateLexems(const std::string& code_);
	static std::string AddSpaces(const std::string& code_);
	auto lexems() const noexcept { return lexems_; }
	void print() const;
private:
	std::vector<std::shared_ptr<LexemInterface>> lexems_;
};
