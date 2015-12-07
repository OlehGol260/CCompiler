#pragma once
#include <string>
#include "lexem.h"
#include <vector>
#include "statement.h"

class Context
{
public:
	explicit Context(int level) : begin_(nullptr), end_(nullptr) {}
	Context(std::shared_ptr<LexemInterface> open, std::shared_ptr<LexemInterface> close) : begin_(open), end_(close) {}
	void AddStatement(std::shared_ptr<Statement> st) { roots_.push_back(st); };
	auto roots() const { return roots_; }

	auto begin() { return begin_; }
	auto end() { return end_; }

	void set_begin(std::shared_ptr<LexemInterface> ch) { begin_ = ch; }
	void set_end(std::shared_ptr<LexemInterface> ch) { end_ = ch; }

private:
	std::shared_ptr<LexemInterface> begin_;
	std::shared_ptr<LexemInterface> end_;
	std::vector<std::shared_ptr<Statement>> roots_;
};
