#pragma once

#include <memory>
#include <vector>

#include "statement.h"
#include "lexeme_interface.h"
#include "typedefs.h"
#include "context.h"

class ReservedWordParser
{
public:

	/*
		Delegate parsing to an appropriate method based on the root type

		@param lexems lexems for current reserved word block

		@return statement which holds a syntax tree
	*/
	static std::shared_ptr<Statement> Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
private:
	/*
		Generate syntax tree for if/else statement

		@param lexems lexems for current reserved word block

		@return statement which holds a syntax tree
	*/
	static std::shared_ptr<Statement> ParseIfElse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
	
	/*
		Generate syntax tree for 'for' loop

		@param lexems lexems for current reserved word block

		@return statement which holds a syntax tree
	*/
	static std::shared_ptr<Statement> ParseLoop(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
	
	/*
		Generate syntax tree for print statement. Print statement holds one node - its body.

		@param lexems lexems for current reserved word block

		@return statement which holds a syntax tree
	*/
	static std::shared_ptr<Statement> ParsePrint(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);

	/*
		Parse statements inside parentheses. Its used primarly for if, while conditions and for loop 3-statement declaration

		@param lexems lexems where need to parse a parenthesised block

		@return context inside outer parentheses
	*/
	static std::shared_ptr<Context> ParseParenthesisedBlock(const std::vector<std::shared_ptr<LexemeInterface>>& cond_lexems);

	/*
		Parse statements inside curly braces. Its used primarly for if, else, while, for blocks

		@param lexems lexems where need to parse a curly braces block

		@return context inside outer curly braces
	*/
	static std::shared_ptr<Context> ParseCurlyBracedBlock(const std::vector<std::shared_ptr<LexemeInterface>>& body_lexems);

	/*
		Find the block for reserved word. This is used, when a reserved word occures inside other block.

		@param begin start of search
		@param end end of search(underlying element will be just after the last checked)

		@return lexems for the reserved word
	*/
	static std::vector<std::shared_ptr<LexemeInterface>> FindReservedWordBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	/*
		Find the outer parenthesised block. This is used to find 'for' loop condition block 

		@param begin start of search
		@param end end of search(underlying element will be just after the last checked)

		@return lexems for the parenthesised block
	*/
	static std::vector<std::shared_ptr<LexemeInterface>> FindParenthesisedBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	/*
		Find the outer curly braces block. This is used to find 'for' loop condition block

		@param begin start of search
		@param end end of search(underlying element will be just after the last checked)

		@return lexems for the curly braces block
	*/
	static std::vector<std::shared_ptr<LexemeInterface>> FindCurlyBracedBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	/*
		Find the nearest print statement.

		@param begin start of search
		@param end end of search(underlying element will be just after the last checked)

		@return lexems for the print statement
	*/
	static std::vector<std::shared_ptr<LexemeInterface>> FindNearestPrint(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	/*
		Find the block with specified open and close symbols. 

		@param begin start of search
		@param end end of search(underlying element will be just after the last checked)
		@param open open symbol of the block
		@param close close symbol of the block

		@return lexems for the block
	*/
	static std::vector<std::shared_ptr<LexemeInterface>> FindBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end, const std::string& open, const std::string& close);
};
