#pragma once
#include <vector>
#include "statement.h"

/*!
	The class holds vector of Statements in a current context and begin and end of this context
*/
class Context
{
public:
	
	Context(std::shared_ptr<LexemeInterface> open, std::shared_ptr<LexemeInterface> close) : begin_(open), end_(close) {}
	Context() : Context(nullptr,nullptr) {}

	/*!
		Add a new statement to the end of the vector
		\param st new statement which have to be added
	*/
	void AddStatement(std::shared_ptr<Statement> st) { roots_.push_back(st); };
	auto roots() const { return roots_; }

	auto begin() { return begin_; }
	auto end() { return end_; }

	void set_begin(std::shared_ptr<LexemeInterface> ch) { begin_ = ch; }
	void set_end(std::shared_ptr<LexemeInterface> ch) { end_ = ch; }

	/*!
		Get the first statement of a context
		\return pointer to the first statement if vector isn't empty, nullptr otherwise
	*/
	std::shared_ptr<Statement> GetFirstRoot() const
	{
		if (roots_.empty()) { return nullptr; }
		return roots_.at(0);
	}
private:
	std::shared_ptr<LexemeInterface> begin_; /*!< begin lexeme*/
	std::shared_ptr<LexemeInterface> end_; /*!< end lexeme*/
	std::vector<std::shared_ptr<Statement>> roots_; /*!< vector of Statements in a current context  */
};
