#include "control_flow_parser.h"

#include "lexem_reserved.h"
#include "grammar.h"
#include "statement_parser.h"

std::shared_ptr<Statement> ControlFlowParser::Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
	auto statement = std::make_shared<Statement>();

	auto reserved_word = std::static_pointer_cast<LexemReserved>(lexems.front());

	auto cbegin = lexems.cbegin();
	auto cend = lexems.cend();

	auto condition_lexems = FindCondition(cbegin + 1, cend); // + 1 - exclude reserved word itself
	auto body_lexems = FindBody(cbegin + condition_lexems.size() + 1, cend); // exclude condition, for optimization purposes

	reserved_word->set_condition(ParseCondition(condition_lexems));
	reserved_word->set_body(ParseBody(body_lexems));

	statement->set_root(reserved_word);
	return statement;
}

std::shared_ptr<Context> ControlFlowParser::ParseCondition(const std::vector<std::shared_ptr<LexemInterface>>& cond_lexems)
{
	if (cond_lexems.empty()) { return nullptr; }

	auto cond_context = std::make_shared<Context>(cond_lexems.front(), cond_lexems.back());
	std::vector<std::shared_ptr<LexemInterface>> statement_lexem_interfs;
	std::shared_ptr<LexemInterface> curr_lexem = nullptr;
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

std::shared_ptr<Context> ControlFlowParser::ParseBody(const std::vector<std::shared_ptr<LexemInterface>>& body_lexems)
{
	if (body_lexems.empty()) { return nullptr; }

	auto body_context = std::make_shared<Context>(body_lexems.front(), body_lexems.back());
	std::vector<std::shared_ptr<LexemInterface>> statement_lexem_interfs;
	std::shared_ptr<LexemInterface> curr_lexem = nullptr;
	auto body_lexems_back = body_lexems.back();
	LexemType curr_lexem_type;

	for (auto it = body_lexems.cbegin()+1; it != body_lexems.cend(); ++it)
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
			auto reserved_word_block = FindControlFlowBlock(it, body_lexems.cend());
			body_context->AddStatement(ControlFlowParser::Parse(reserved_word_block));
			statement_lexem_interfs.clear();
			it += reserved_word_block.size() - 1;
		}
	}

	return body_context;
}

std::vector<std::shared_ptr<LexemInterface>> ControlFlowParser::FindControlFlowBlock(lexem_interfaces_iter cbegin, lexem_interfaces_iter cend)
{
	std::vector<std::shared_ptr<LexemInterface>> result;
	std::shared_ptr<LexemInterface> curr_lexem = nullptr;
	auto opened_braces_count = 0;
	auto found_finite_block = false; //TODO: ask Max if it's better to do so rather than use 'break' 
	LexemType curr_lexem_type;

	for (auto it = cbegin; !found_finite_block && it != cend; ++it)
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
			found_finite_block = true;
		}
	}
	return result;
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
	auto opened_bracket_count = 0;
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
			opened_bracket_count++;
		}
		if (current_token_value == close)
		{
			opened_bracket_count--;
			if (!opened_bracket_count) { is_found = true; }
		}
		result_lexems.push_back(current_token);
	}
	return result_lexems;
}
