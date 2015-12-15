#pragma once

#include <memory>
#include <sstream>
#include "context.h"

/*!
	The class generate english representation the origin program
*/
class Translator
{
public:
	/*!
		An entry point for the translator. Iterate over each statement and forward
		the translation to a corresponding method
		\param main_context syntax tree of the origin program
	*/
	void Translate(std::vector<std::shared_ptr<Statement>> main_context);

	/*!
		Prints the content of ss_ to the standard output
	*/
	void Print() const;

	/*!
		Clears the ss_ member
	*/
	void Clear();

	/*!
		Saves the current content of ss_ to the file to which filepath points
		\param filepath path to the output file in the system
	*/
	void SaveToFile(const std::string& filepath) const;
private:
	/*!
		Translates mathematic and boolean operations, assignment,
		variables and sqrt
		\param li root lexeme in the syntax tree
		\return string represantation of the current syntax tree
	*/
	std::string TranslateExpression(std::shared_ptr<LexemeInterface> li);

	/*!
		Translates an if-else statement
		\param li root node of the if a syntax tree.
	*/
	void TranslateIfElse(std::shared_ptr<LexemeInterface> li);

	/*!
		Translates a variable declaration. li param holds 2 leaves - left is type, right is
		variable name
		\param li root node of the var declarartion syntax tree.
	*/
	void TranslateVarDeclaration(std::shared_ptr<LexemeInterface> li);

	/*!
		Translates a print statement. Print statement can hold either string or variable or value
		\param li root node of the if a syntax tree.
	*/
	void TranslatePrint(std::shared_ptr<LexemeInterface> li);

	/*!
		Translates a for loop. For loop is restricted with 3 parameters.
		\param li root node of the for loop syntax tree.
	*/
	void TranslateForLoop(std::shared_ptr<LexemeInterface> li);

	/*!
		Translates a while loop
		\param li root node of the while loop  syntax tree.
	*/
	void TranslateWhileLoop(std::shared_ptr<LexemeInterface> li);

	/*!
		Translates an assignment statement. There are no evaluations here.
		Actually, this method delegate li to the TranslateExpression
		\param li root node of the if syntax tree.
	*/
	void TranslateAssignment(std::shared_ptr<LexemeInterface> li);

	/*!
		Translates a context if it's not empty. The result of the translation is covered with
		'msg' and 'begin' at the beginning and 'end' at the end. This is a convinient way of parsing
		a block of if/else, for and while loops
		\param context a context which will be parsed
		\param msg a string which is followed by "begin" and put to ss_ before translating context
	*/
	void TranslateIfNotEmpty(std::shared_ptr<Context> context, const std::string& msg);
private:
	std::stringstream ss_; /*!! a stream which holds the result of translating the origin code */
};
