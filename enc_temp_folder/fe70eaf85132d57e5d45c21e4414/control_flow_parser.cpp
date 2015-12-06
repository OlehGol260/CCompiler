#include "control_flow_parser.h"

#include "lexem_reserved.h"
#include "grammar.h"
#include "statement_parser.h"

std::shared_ptr<Statement> ControlFlowParser::Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
	auto statement = std::make_shared<Statement>();

	auto reserved_word = std::static_pointer_cast<LexemReserved>(lexems.front());

	auto condition_lexems = FindCondition(lexems.cbegin() + 1, lexems.cend()); // +1 - exclude reserved word itself
	auto body_lexems = FindBody(lexems.cbegin() + condition_lexems.size() + 1, lexems.cend()); // exclude condition

	reserved_word->set_condition(ParseCondition(condition_lexems));
	reserved_word->set_body(ParseBody(body_lexems));

	statement->set_root(reserved_word);
	return statement;
}

std::shared_ptr<Context> ControlFlowParser::ParseCondition(const std::vector<std::shared_ptr<LexemInterface>>& cond_lexems)
{
	if (cond_lexems.empty()) { return nullptr; }

	auto cond_context = std::make_shared<Context>(cond_lexems.front(), cond_lexems.back());
	std::vector<std::shared_ptr<LexemInterface>> statement;
	std::shared_ptr<LexemInterface> lexem = nullptr;
	for (auto it = cond_lexems.cbegin(); it != cond_lexems.cend(); ++it)
	{
		lexem = *it;
		statement.push_back(lexem);
		if (Grammar::IsPunctuator(lexem->type()) || cond_lexems.back() == lexem) // TODO: WORKAROUND for now
		{
			cond_context->AddStatement(StatementParser::Parse(statement));
			statement.clear();
		}
	}
	return cond_context;
}

std::shared_ptr<Context> ControlFlowParser::ParseBody(const std::vector<std::shared_ptr<LexemInterface>>& body_lexems)
{
	return nullptr;
}

std::vector<std::shared_ptr<LexemInterface>> ControlFlowParser::FindCondition(lexem_interfaces_iter begin, lexem_interfaces_iter end)
{
	return FindBlock(begin, end, Grammar::open_parenthesis(), Grammar::close_parenthesis());
}

std::vector<std::shared_ptr<LexemInterface>> ControlFlowParser::FindBody(lexem_interfaces_iter begin, lexem_interfaces_iter end)
{
	return FindBlock(begin, end, Grammar::open_brace(), Grammar::close_brace());
}

std::vector<std::shared_ptr<LexemInterface>> ControlFlowParser::FindBlock(lexem_interfaces_iter begin, lexem_interfaces_iter end, const std::string& open, const std::string& close)
{
	auto opened_count = 0;
	auto is_found = false;
	std::shared_ptr<LexemInterface> current_token = nullptr;
	std::string current_token_value;
	std::vector<std::shared_ptr<LexemInterface>> result_lexems;
	for (auto it = begin; !is_found && it != end; ++it)
	{
		current_token = *it;
		current_token_value = current_token->value();
		if (current_token_value == open)
		{
			opened_count++;
		}
		if (current_token_value == close)
		{
			opened_count--;
			if (!opened_count) { is_found = true; }
		}
		result_lexems.push_back(current_token);
	}
	return result_lexems;
}
