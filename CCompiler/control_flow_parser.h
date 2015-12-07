#pragma once

#include <memory>
#include <vector>

#include "statement.h"
#include "lexeme_interface.h"
#include "typedefs.h"
#include "context.h"

class ControlFlowParser
{
public:
	static std::shared_ptr<Statement> Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
private:
	static std::shared_ptr<Context> ParseCondition(const std::vector<std::shared_ptr<LexemeInterface>>& cond_lexems);
	static std::shared_ptr<Context> ParseBody(const std::vector<std::shared_ptr<LexemeInterface>>& body_lexems);

	static std::vector<std::shared_ptr<LexemeInterface>> FindControlFlowStatement(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemeInterface>> FindCondition(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemeInterface>> FindBody(lexeme_interfaces_iter begin, lexeme_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemeInterface>> FindBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end, const std::string& open, const std::string& close);
};
