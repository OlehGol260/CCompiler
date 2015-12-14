#include "reserved_word_parser.h"

#include "grammar.h"
#include "statement_parser.h"
#include "lexeme_loop.h"
#include "lexeme_func.h"
#include <assert.h>
#include "lexeme_condition.h"

std::shared_ptr<Statement> ReservedWordParser::Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems)
{
	std::shared_ptr<Statement> statement = nullptr;

	auto reserved_word = lexems.front();
	
	switch(reserved_word->type())
	{
	case LT::kIf:
		statement = ParseIfElse(lexems);
		break;
	case LT::kFor:
	case LT::kWhile:
		statement = ParseLoop(lexems);
		break;
	case LT::kPrint:
		statement = ParsePrint(lexems);
		break;
	}

	return statement;
}

std::shared_ptr<Statement> ReservedWordParser::ParseIfElse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems)
{
	assert(!lexems.empty() && "Trying to parse condition: empty lexems");

	auto cond_st = std::make_shared<Statement>();

	auto lexems_front = lexems.front();
	assert(Grammar::IsIf(lexems_front->type()));

	auto if_lexeme = std::static_pointer_cast<LexemeCondition>(lexems_front);
	auto cbegin = lexems.cbegin();
	auto cend = lexems.cend();


	auto condition_block = FindParenthesisedBlock(cbegin, cend);
	auto condition_context = ParseParenthesisedBlock(condition_block);
	if_lexeme->set_condition(condition_context->GetFirstRoot());

	auto if_curly_braced_block = FindCurlyBracedBlock(cbegin, cend);
	if_lexeme->set_if_body(ParseCurlyBracedBlock(if_curly_braced_block));


	//5 : 1 - if word, 2 - parentheses, 2 - curly braces
	//if else is present, else_cbegin point at it
	auto else_cbegin = cbegin + condition_block.size() + if_curly_braced_block.size() + 5;

	if (else_cbegin != cend)
	{
		assert(Grammar::IsElse((*else_cbegin)->type()));
		auto else_curly_braced_block = FindCurlyBracedBlock(else_cbegin, cend);
		if_lexeme->set_else_body(ParseCurlyBracedBlock(else_curly_braced_block));
	}
	cond_st->set_root(if_lexeme);
	return cond_st;
}

std::shared_ptr<Statement> ReservedWordParser::ParseLoop(const std::vector<std::shared_ptr<LexemeInterface>>& lexems)
{
	assert(!lexems.empty() && "Trying to parse loop: empty lexems");

	auto loop_st = std::make_shared<Statement>();

	auto lexems_front = lexems.front();
	assert(Grammar::IsLoop(lexems_front->type()));

	auto loop_lexem = std::static_pointer_cast<LexemeLoop>(lexems_front);
	auto cbegin = lexems.cbegin();
	auto cend = lexems.cend();

	loop_lexem->set_condition(ParseParenthesisedBlock(FindParenthesisedBlock(cbegin, cend)));
	loop_lexem->set_body(ParseCurlyBracedBlock(FindCurlyBracedBlock(cbegin, cend)));

	loop_st->set_root(loop_lexem);
	return loop_st;
}

std::shared_ptr<Statement> ReservedWordParser::ParsePrint(const std::vector<std::shared_ptr<LexemeInterface>>& lexems)
{
	assert(!lexems.empty() && "Trying to parse print: empty lexems");
	auto print_st = std::make_shared<Statement>();
	auto lexems_front = lexems.front();

	assert(Grammar::IsPrint(lexems_front->type()));

	auto print = std::static_pointer_cast<LexemeFunc>(lexems_front);

	auto print_block = FindParenthesisedBlock(lexems.cbegin(), lexems.cend());
	auto print_block_root = ParseParenthesisedBlock(print_block)->GetFirstRoot();
	assert(print_block_root && "Print statement cannot be empty");
	print->set_body(print_block_root->root());
	print_st->set_root(print);
	auto print_st_semicolon = lexems.back();
	assert(Grammar::IsPunctuator(print_st_semicolon->type()) && "Print statement must end with semicolon");
	print_st->set_punctuator(print_st_semicolon);
	return print_st;
}

std::shared_ptr<Context> ReservedWordParser::ParseParenthesisedBlock(const std::vector<std::shared_ptr<LexemeInterface>>& cond_lexems)
{
	assert(!cond_lexems.empty() && "Trying to parse parenthesised block: empty lexems");

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

std::shared_ptr<Context> ReservedWordParser::ParseCurlyBracedBlock(const std::vector<std::shared_ptr<LexemeInterface>>& body_lexems)
{
	assert(!body_lexems.empty() && "Trying to parse curly braced block: empty lexems");

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
			body_context->AddStatement(ReservedWordParser::Parse(reserved_word_block));
			statement_lexem_interfs.clear();
			it += reserved_word_block.size() - 1; // WATCH OUT HERE
		}
	}

	return body_context;
}

std::vector<std::shared_ptr<LexemeInterface>> ReservedWordParser::FindControlFlowStatement(lexeme_interfaces_iter cbegin, lexeme_interfaces_iter cend)
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

std::vector<std::shared_ptr<LexemeInterface>> ReservedWordParser::FindParenthesisedBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end)
{
	return FindBlock(begin, end, Grammar::open_parenthesis(), Grammar::close_parenthesis());
}

std::vector<std::shared_ptr<LexemeInterface>> ReservedWordParser::FindCurlyBracedBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end)
{
	return FindBlock(begin, end, Grammar::open_brace(), Grammar::close_brace());
}

std::vector<std::shared_ptr<LexemeInterface>> ReservedWordParser::FindNearestPrint(lexeme_interfaces_iter begin, lexeme_interfaces_iter end)
{
	std::vector<std::shared_ptr<LexemeInterface>> result = {};

	for (auto it = begin; it != end; ++it)
	{
		
	}
	return result;
}

std::vector<std::shared_ptr<LexemeInterface>> ReservedWordParser::FindBlock(lexeme_interfaces_iter begin, lexeme_interfaces_iter end, const std::string& open, const std::string& close)
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
