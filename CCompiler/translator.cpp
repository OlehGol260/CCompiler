#include "translator.h"
#include <assert.h>
#include "typedefs.h"
#include "err_msg.h"
#include "lexeme_condition.h"
#include "grammar.h"
#include <iostream>

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

		auto var_init = st->var_init();
		if (var_init)
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
}

void Translator::TranslateForLoop(std::shared_ptr<LexemeInterface> li)
{
}

void Translator::TranslateWhileLoop(std::shared_ptr<LexemeInterface> li)
{
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
