#include "translator.h"
#include <assert.h>
#include <fstream>
#include <iostream>

#include "typedefs.h"
#include "lexeme_condition.h"
#include "grammar.h"

#include "lexeme_func.h"
#include "lexeme_loop.h"

void Translator::Translate(std::vector<std::shared_ptr<Statement>> main_context)
{
	std::shared_ptr<LexemeInterface> st_root;
	std::shared_ptr<Statement> st;
	LexemeType lexeme_type;
	for (auto it = main_context.cbegin(); it != main_context.cend(); ++it)
	{
		st = *it;
		st_root = st->root();
		lexeme_type = st_root->type();

		if (auto var_init = st->var_init())
		{
			TranslateVarDeclaration(var_init);
		}

		switch (lexeme_type)
		{
		case LT::kIf:
			TranslateIf(st_root);
			break;
		case LT::kAssignment:
			TranslateAssignment(st_root);
			break;
		case LT::kPrint:
			TranslatePrint(st_root);
			break;
		case LT::kFor:
			TranslateForLoop(st_root);
			break;
		case LT::kWhile:
			TranslateWhileLoop(st_root);
			break;
		}
		ss_ << "\n";
	}
}

void Translator::TranslateIf(std::shared_ptr<LexemeInterface> li)
{
	assert(li && "Tried to translate an empty lexeme");
	auto if_lexeme = std::static_pointer_cast<LexemeCondition>(li);

	auto cond_lexeme = if_lexeme->conidition();
	ss_ << "If value of " << TranslateExpression(cond_lexeme->root()) << " is true ";

	TranslateIfNotEmpty(if_lexeme->if_body(), "then");
	TranslateIfNotEmpty(if_lexeme->else_body(), "else");
}

void Translator::TranslateVarDeclaration(std::shared_ptr<LexemeInterface> li)
{
	assert(li && "Tried to translate an empty lexeme");
	auto var_decl = std::static_pointer_cast<Lexeme>(li);
	ss_ << "Variable '" << var_decl->right()->value() << "' of type " << var_decl->left()->value();
}

void Translator::TranslatePrint(std::shared_ptr<LexemeInterface> li)
{
	assert(li && "Tried to translate an empty lexeme");
	auto print_func = std::static_pointer_cast<LexemeFunc>(li);
	auto body = print_func->body();
	assert(body && "Body of a print statement cannot be empty");
	
	ss_ << "Print " << (Grammar::IsVariable(body->type()) ? "value of variable " : "") << body->value();
}

void Translator::TranslateForLoop(std::shared_ptr<LexemeInterface> li)
{
	assert(li && "Tried to translate an empty lexeme");
	auto loop_it = std::static_pointer_cast<LexemeLoop>(li);
	auto condition = loop_it->condition();
	assert(condition && "condition cannot be empty");
	auto condition_roots = condition->roots();
	auto counter_st = condition_roots.at(0);
	auto counter_lexem = counter_st->root();
	ss_ << "For ";
	if (Grammar::IsAssignment(counter_lexem->type()))
	{
		if (auto counter_init = counter_st->var_init())
		{
			TranslateVarDeclaration(counter_init);
		}
	}
	TranslateAssignment(counter_lexem);

	ss_ << " while true " << TranslateExpression(condition_roots.at(1)->root()) + " and at each step perform ";
	TranslateAssignment(condition_roots.at(2)->root());
	
	TranslateIfNotEmpty(loop_it->body(), " DO ");
}

void Translator::TranslateWhileLoop(std::shared_ptr<LexemeInterface> li)
{
	assert(li && "Tried to translate an empty lexeme");
	auto loop_it = std::static_pointer_cast<LexemeLoop>(li);

	auto condition = loop_it->condition();
	assert(condition && "condition cannot be empty");
	std::string msg = "while true " + TranslateExpression(condition->roots().at(0)->root()) + " do";
	TranslateIfNotEmpty(loop_it->body(), msg);
}

void Translator::TranslateAssignment(std::shared_ptr<LexemeInterface> li)
{
	ss_ << TranslateExpression(li);
}

void Translator::TranslateIfNotEmpty(std::shared_ptr<Context> context, const std::string& msg)
{
	if (context)
	{
		ss_ << msg << "\nbegin\n";
 		Translate(context->roots());
		ss_ << "end\n";
	}
}

std::string Translator::TranslateExpression(std::shared_ptr<LexemeInterface> li)
{
	assert(li && "Tried to translate an empty lexeme");

	auto li_type = li->type();
	auto li_value = li->value();
	if (Grammar::IsBinaryOperator(li_type))
	{
		auto lexeme = std::static_pointer_cast<Lexeme>(li);
		return " (" + TranslateExpression(lexeme->left()) + Grammar::GetTextByOperation(li_value) + TranslateExpression(lexeme->right()) + ") ";
	}
	if (Grammar::IsVariable(li_type))
	{
		return "variable " + li_value;
	}
	return li_value;
}

void Translator::Print() const
{
	std::cout << ss_.str() <<std::endl; 
}

void Translator::SaveToFile(const std::string& filepath) const
{
	std::ofstream file(filepath);
	if (file.is_open())
	{
		file << ss_.rdbuf();
		file.close();
	}
}
