#pragma once
#include <string>
#include <vector>
#include "lexem.h"

class Lexer
{
public:
	void GenerateLexems(const std::string& code_);
	static std::string AddSpaces(const std::string& code_);

	void print() const;
private:
	std::vector<std::shared_ptr<Lexem>> lexems_;
};
