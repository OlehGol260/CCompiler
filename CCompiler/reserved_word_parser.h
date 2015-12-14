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
	static std::shared_ptr<Statement> Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
private:
	static std::shared_ptr<Statement> ParseIfElse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
	
	static std::shared_ptr<Statement> ParseLoop(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
	
	static std::shared_ptr<Statement> ParsePrint(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);

	static std::shared_ptr<Context> ParseParenthesisedBlock(const std::vector<std::shared_ptr<LexemeInterface>>& cond_lexems);

	static std::shared_ptr<Context> ParseCurlyBracedBlock(const std::vector<std::shared_ptr<LexemeInterface>>& body_lexems);

	static std::vector<std::shared_ptr<LexemeInterface>> FindControlFlowStatement(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemeInterface>> FindParenthesisedBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemeInterface>> FindCurlyBracedBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemeInterface>> FindNearestPrint(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemeInterface>> FindBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end, const std::string& open, const std::string& close);


};
