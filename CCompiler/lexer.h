#pragma once
#include <string>
#include <vector>
#include "lexeme.h"

/*
	Perform lexical analysis of the given code generating lexems which then will be deligated to the parser
*/
class Lexer
{
public:
	/*!
		Parse the given code generating a vector of lexems
		\param code_ string representation of the code 
	*/
	void Parse(const std::string& code_);

	/*!
		 Add spaces between operators and handle double qoutes used in print statement
		 \param code_ part of the whole code which need to be processed
		 \return processed string
	*/
	static std::string AddSpaces(const std::string& code_);

	/*!
		Getter for lexems
		\return lexems_
	*/
	auto lexems() const noexcept { return lexems_; }
private:
	std::vector<std::shared_ptr<LexemeInterface>> lexems_; /*!< vector of LexemeInterfaces. Elements can hold any other lexeme derived type*/
};
