#include "statement_parser.h"

#include <iostream>

#include "grammar.h"


std::shared_ptr<Statement> StatementParser::Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
	if (lexems.empty()) { return nullptr; }

	auto statement = std::make_shared<Statement>();
	auto is_var_decl = 0; //do not include type to the analyzies, if appeares
	auto is_punctuater = 0; // do not include punctuator, if appeares
	if (lexems.at(0)->type() == LT::kType)
	{
		//is_var_decl = 1; TODO: THIS IS CORRECT, for debug remove init
		is_var_decl = 3;
		auto var_init = std::make_shared<Lexem>(LT::kVarDeclaration, "DEF");
		var_init->set_left(std::static_pointer_cast<Lexem>(lexems.at(0)));
		var_init->set_right(std::static_pointer_cast<Lexem>(lexems.at(1)));
		statement->set_var_init(var_init);
	}
	if (lexems.size() > 2)
	{
		is_punctuater = 1;
		statement->set_punctuator(lexems.back());
	}

	statement->set_root(InnerMathStatParse(nullptr, WhereAttachCh::kUnknown, lexems.rbegin() + is_punctuater, lexems.rend() - is_var_decl));

	return statement;
}

std::shared_ptr<LexemInterface> StatementParser::InnerMathStatParse(std::shared_ptr<LexemInterface> parent, const WhereAttachCh&& side, lexem_interfaces_reverse_iter rbegin, lexem_interfaces_reverse_iter rend)
{
	//TODO: ASSIGNMENT

	auto a = std::min_element(rbegin, rend, [](std::shared_ptr<LexemInterface> l1, std::shared_ptr<LexemInterface> l2)
	{
		return l1->level() < l2->level();
	});
	auto curr_level = (*a)->level();
	auto current_token = *rbegin; // if found needed operator, this will hold this operator
	auto found_needed_op = false;

	for (auto it_math = Grammar::math_operators().cbegin(); !found_needed_op && it_math != Grammar::math_operators().cend(); ++it_math)
	{
		for (auto it_tok = rbegin; !found_needed_op && it_tok != rend; ++it_tok)
		{
			current_token = *it_tok;
			auto current_token_value = current_token->value();
			if (curr_level == current_token->level() && current_token->type() == LT::kMathOp && current_token->value() == *it_math)
			{
				
				std::cout << current_token->value() << std::endl;
				
				InnerMathStatParse(current_token, WhereAttachCh::kLeft, it_tok + 1, rend);
				InnerMathStatParse(current_token, WhereAttachCh::kRight, rbegin, it_tok - 1);

				found_needed_op = true;
			}
		}
	}
	if (parent)
	{
		auto parent_ = std::static_pointer_cast<Lexem>(parent);
		if (side == WhereAttachCh::kLeft)
		{
			parent_->set_left(current_token);
		}
		else
		{
			parent_->set_right(current_token);
		}
		parent = parent_;
		return parent;
	} 
	return current_token;	
}
