#pragma once

#include <vector>
#include <memory>

#include "lexem_interface.h"
#include "typedefs.h"
#include "statement.h"

class StatementParser
{
public:
	static std::shared_ptr<Statement> Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems);
private:
	static std::shared_ptr<LexemInterface> InnerStatParse(std::shared_ptr<LexemInterface> parent, const WhereAttachCh&& side, lexem_interfaces_reverse_iter rbegin, lexem_interfaces_reverse_iter rend);
	static int GetLowestLevel(lexem_interfaces_reverse_iter rbegin, lexem_interfaces_reverse_iter rend);
};
