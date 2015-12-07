#pragma once

#include <memory>
#include <vector>

#include "statement.h"
#include "lexem_interface.h"
#include "typedefs.h"
#include "context.h"

class ControlFlowParser
{
public:
	static std::shared_ptr<Statement> Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems);
private:
	static std::shared_ptr<Context> ParseCondition(const std::vector<std::shared_ptr<LexemInterface>>& cond_lexems);
	static std::shared_ptr<Context> ParseBody(const std::vector<std::shared_ptr<LexemInterface>>& body_lexems);

	static std::vector<std::shared_ptr<LexemInterface>> FindControlFlowStatement(lexem_interfaces_iter begin, lexem_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemInterface>> FindCondition(lexem_interfaces_iter begin, lexem_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemInterface>> FindBody(lexem_interfaces_iter begin, lexem_interfaces_iter end);

	static std::vector<std::shared_ptr<LexemInterface>> FindBlock(lexem_interfaces_iter begin, lexem_interfaces_iter end, const std::string& open, const std::string& close);
};
