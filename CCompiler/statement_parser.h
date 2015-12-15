#pragma once

#include <vector>
#include <memory>

#include "lexeme_interface.h"
#include "typedefs.h"
#include "statement.h"

/*!
	Generate statement for expressions. The main aim to correctly generate the tree for binary operations
	based on operator precedence and parentheses.
*/
class StatementParser
{
public:
	/*!
		Method receive lexems for the current statement. Add variable declaration and a punctuator if they are present in the vector
		\param lexems vector of lexems for current statement
		\return pointer to created statement
	*/
	static std::shared_ptr<Statement> Parse(const std::vector<std::shared_ptr<LexemeInterface>>& lexems);
private:
	/*!
		This function will operate recursively over the lexems and find appropriate lexeme in range of [rbegin, rend) and attach it to the parent.
		Node can be variable or value or binary operator or sqrt function
		\param parent lexeme where a lexem will be attached
		\param side show where to attach a lexeme
		\param rbegin object of reverse iterator(vector::rbegin())
		\param rend object of reverse iterator(vector::rend())
		\return root lexeme of the statement
	*/
	static std::shared_ptr<LexemeInterface> InnerStatParse(std::shared_ptr<LexemeInterface> parent, const WhereAttachCh&& side, lexeme_interfaces_reverse_iter rbegin, lexeme_interfaces_reverse_iter rend);

	/*!
		Get the lowest level of a lexem in the current range [rbegin, rend)
		This is used in parentheses stuff, when parentheses affect the tree structure and the whole operator precedence
		\param rbegin object of reverse iterator(vector::rbegin())
		\param rend object of reverse iterator(vector::rend())
		\return the lowest value
	*/
	static int GetLowestLevel(lexeme_interfaces_reverse_iter rbegin, lexeme_interfaces_reverse_iter rend);
};
