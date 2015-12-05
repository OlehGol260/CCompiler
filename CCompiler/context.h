#pragma once
#include <string>
#include "lexem.h"
#include <vector>
#include "statement.h"

class Context
{
public:
	explicit Context(int level) : begin_(nullptr), end_(nullptr), level_(level) {}
	void AddStatement(std::shared_ptr<Statement> st) { roots_.push_back(st); };
	auto roots() const { return roots_; }
	auto level() const { return level_; }

	auto begin() { return begin_; }
	auto end() { return end_; }

	void set_begin(std::shared_ptr<Lexem> ch) { begin_ = ch; }
	void set_end(std::shared_ptr<Lexem> ch) { end_ = ch; }

private:
	std::shared_ptr<Lexem> begin_;
	std::shared_ptr<Lexem> end_;
	std::vector<std::shared_ptr<Statement>> roots_;
	int level_;
};
