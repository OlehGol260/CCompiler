#include "control_flow_parser.h"

#include "grammar.h"
#include "statement_parser.h"
#include "lexeme_loop.h"

std::shared_ptr<Statement> ControlFlowParser::Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems)
{
	auto statement = std::make_shared<Statement>();

	auto reserved_word = std::static_pointer_cast<LexemeLoop>(lexems.front());

	auto cbegin = lexems.cbegin();
	auto cend = lexems.cend();

	reserved_word->set_condition(ParseCondition(FindCondition(cbegin, cend)));
	reserved_word->set_body(ParseBody(FindBody(cbegin, cend)));

	statement->set_root(reserved_word);
	return statement;
}

std::shared_ptr<Context> ControlFlowParser::ParseCondition(const std::vector<std::shared_ptr<LexemeInterface>>& cond_lexems)
{
	if (cond_lexems.empty()) { return nullptr; }

	auto cond_context = std::make_shared<Context>(cond_lexems.front(), cond_lexems.back());
	std::vector<std::shared_ptr<LexemeInterface>> statement_lexem_interfs;
	std::shared_ptr<LexemeInterface> curr_lexem = nullptr;
	auto cond_lexems_back = cond_lexems.back();
	for (auto it = cond_lexems.cbegin(); it != cond_lexems.cend(); ++it)
	{
		curr_lexem = *it;
		statement_lexem_interfs.push_back(curr_lexem);
		if (Grammar::IsPunctuator(curr_lexem->type()) || cond_lexems_back == curr_lexem) // the second check is for the last statement in for loop block - changing the counter - it may stay without ';'
		{
			cond_context->AddStatement(StatementParser::Parse(statement_lexem_interfs));
			statement_lexem_interfs.clear();
		}
	}
	return cond_context;
}

std::shared_ptr<Context> ControlFlowParser::ParseBody(const std::vector<std::shared_ptr<LexemeInterface>>& body_lexems)
{
	if (body_lexems.empty()) { return nullptr; }

	auto body_context = std::make_shared<Context>(body_lexems.front(), body_lexems.back());
	std::vector<std::shared_ptr<LexemeInterface>> statement_lexem_interfs;
	std::shared_ptr<LexemeInterface> curr_lexem = nullptr;
	auto body_lexems_back = body_lexems.back();
	LexemeType curr_lexem_type;

	for (auto it = body_lexems.cbegin(); it != body_lexems.cend(); ++it)
	{
		curr_lexem = *it;
		curr_lexem_type = curr_lexem->type();
		statement_lexem_interfs.push_back(curr_lexem);
		if (Grammar::IsPunctuator(curr_lexem_type))
		{
			body_context->AddStatement(StatementParser::Parse(statement_lexem_interfs));
			statement_lexem_interfs.clear();
		} 
		if (Grammar::IsReservedWord(curr_lexem_type))
		{
			auto reserved_word_block = FindControlFlowStatement(it, body_lexems.cend());
			body_context->AddStatement(ControlFlowParser::Parse(reserved_word_block));
			statement_lexem_interfs.clear();
			it += reserved_word_block.size();
		}
	}

	return body_context;
}

std::vector<std::shared_ptr<LexemeInterface>> ControlFlowParser::FindControlFlowStatement(lexeme_interfaces_iter cbegin, lexeme_interfaces_iter cend)
{
	std::vector<std::shared_ptr<LexemeInterface>> result;
	std::shared_ptr<LexemeInterface> curr_lexem = nullptr;
	auto opened_braces_count = 0;
	LexemeType curr_lexem_type;

	for (auto it = cbegin; it != cend; ++it)
	{
		curr_lexem = *it;
		curr_lexem_type = curr_lexem->type();
		result.push_back(curr_lexem);
		if (Grammar::IsOpenBrace(curr_lexem_type))
		{
			opened_braces_count++;
		}
		if (Grammar::IsCloseBrace(curr_lexem_type) && !--opened_braces_count)
		{
			break;
		}
	}
	return result;
}

std::vector<std::shared_ptr<LexemeInterface>> ControlFlowParser::FindCondition(lexeme_interfaces_iter begin, lexeme_interfaces_iter end)
{
	return FindBlock(begin, end, Grammar::open_parenthesis(), Grammar::close_parenthesis());
}

std::vector<std::shared_ptr<LexemeInterface>> ControlFlowParser::FindBody(lexeme_interfaces_iter begin, lexeme_interfaces_iter end)
{
	return FindBlock(begin, end, Grammar::open_brace(), Grammar::close_brace());
}

std::vector<std::shared_ptr<LexemeInterface>> ControlFlowParser::FindBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end, const std::string& open, const std::string& close)
{
	auto opened_bracket_count = 0;
	std::shared_ptr<LexemeInterface> current_token = nullptr;
	std::string current_token_value;
	std::vector<std::shared_ptr<LexemeInterface>> result_lexems;

	for (auto it = begin;it != end; ++it)
	{
		current_token = *it;
		current_token_value = current_token->value();
		if (current_token_value == open && !opened_bracket_count++)
		{
			continue; // outer open bracket won't be pushed to a result_lexems
		}
		if (current_token_value == close && !--opened_bracket_count)
		{
			break;
		}
		if (opened_bracket_count) // only if in scope.
		{
			result_lexems.push_back(current_token);
		}	
	}
	return result_lexems;
}
