#pragma once

#include <vector>
#include <memory>

#include "lexeme_interface.h"
#include "typedefs.h"
#include "statement.h"

class StatementParser
{
public:
	static std::shared_ptr<Statement> Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
private:
	static std::shared_ptr<LexemeInterface> InnerStatParse(std::shared_ptr<LexemeInterface> parent, const WhereAttachCh&& side, lexeme_interfaces_reverse_iter rbegin, lexeme_interfaces_reverse_iter rend);
	static int GetLowestLevel(lexeme_interfaces_reverse_iter rbegin, lexeme_interfaces_reverse_iter rend);
	static std::shared_ptr<LexemeInterface> ParseFunc(std::shared_ptr<LexemeInterface>);
};
