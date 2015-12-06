#include "statement.h"

#include <iostream>
#include <string>
#include "grammar.h"

void Statement::Print() const
{
	//TODO: assignment and type

	InnerPrint(root_);

}

void Statement::InnerPrint(std::shared_ptr<LexemInterface> lexem) const
{
	if (!lexem)
	{
		return;
	}

	auto lexem_ = std::static_pointer_cast<Lexem>(lexem);

	auto value = lexem->value();
	if (!(Grammar::IsVariable(lexem->type()) || Grammar::IsLiteral(lexem_->type())))
	{
		std::cout << "(";
	}
	
	InnerPrint(lexem_->left());
	std::cout << lexem->value();
	InnerPrint(lexem_->right());
	if (!(Grammar::IsVariable(lexem->type()) || Grammar::IsLiteral(lexem_->type())))
	{
		std::cout << ")";
	}
	
}
