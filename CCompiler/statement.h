#pragma once

#include <memory>
#include "lexeme_interface.h"
#include "lexeme.h"

/*
	Contains the root node of the tree, the statement punctuater(it's a semicolon)
	and a variable declaration syntax tree in case if a statement contains  
*/
class Statement
{
public:
	/*
		Initialize an empty statement
	*/
	Statement() : Statement(nullptr) {}
	/*
		Set root_ member with lexem

		@param lexem root node of the statement
	*/
	explicit Statement(std::shared_ptr<LexemeInterface> lexem) : root_(lexem), punctuator_(nullptr) {}

	/*
		Default destructor
	*/
	~Statement() = default;

	/*
		Checks if current statement has a punctuator
	*/
	bool HasPunctuator() const noexcept { return punctuator_ != nullptr; }

	/*
		Getter for punctuator

		@return punctuator lexeme
	*/
	auto punctuator() const noexcept { return punctuator_; }
	/*
		Setter for punctuator
		
		@param p punctuator lexeme
	*/
	void set_punctuator(std::shared_ptr<LexemeInterface> p) { punctuator_ = std::static_pointer_cast<Lexeme>(p); }

	/*
		Setter for root

		@param p root lexeme
	*/
	void set_root(std::shared_ptr<LexemeInterface> r) { root_ = r; }

	/*
		Getter for root

		@return root lexeme
	*/
	auto root() { return root_; }

	/*
	Setter for variable declaration

	@param p root lexeme
	*/
	void set_var_decl(std::shared_ptr<Lexeme> i) { var_decl_ = i; }

	/*
	Getter for root

	@return root lexeme
	*/
	auto var_decl() const noexcept { return var_decl_; }

private:
	std::shared_ptr<LexemeInterface> root_; /*!< hold Lexeme object with the actual syntax tree */
	std::shared_ptr<LexemeInterface> punctuator_; /*!< hold Lexeme object with the punctuator */

	//workaround to work with variable initialization
	//this var will hold tree of type
	//	left node - type lexem
	//	right node - var lexem
	std::shared_ptr<Lexeme> var_decl_; /*!< hold Lexeme object with the syntax tree for variable declarartion */
};
