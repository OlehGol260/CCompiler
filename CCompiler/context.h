#pragma once
#include <vector>
#include "statement.h"

class Context
{
public:
	
	Context(std::shared_ptr<LexemeInterface> open, std::shared_ptr<LexemeInterface> close) : begin_(open), end_(close) {}
	Context() : Context(nullptr,nullptr) {}
	void AddStatement(std::shared_ptr<Statement> st) { roots_.push_back(st); };
	auto roots() const { return roots_; }

	auto begin() { return begin_; }
	auto end() { return end_; }

	void set_begin(std::shared_ptr<LexemeInterface> ch) { begin_ = ch; }
	void set_end(std::shared_ptr<LexemeInterface> ch) { end_ = ch; }

	std::shared_ptr<Statement> GetFirstRoot() const
	{
		if (roots_.empty()) { return nullptr; }
		return roots_.at(0);
	}
private:
	std::shared_ptr<LexemeInterface> begin_;
	std::shared_ptr<LexemeInterface> end_;
	std::vector<std::shared_ptr<Statement>> roots_;
};
