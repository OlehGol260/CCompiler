#pragma once
#include <string>

class Context
{
public:
	explicit Context(int level) : begin_(""), end_(""), content_(""), level_(level) {}
	void append(const std::string& lexem) { content_ += lexem + " "; };
	auto content() const { return content_; }
	auto level() const { return level_; }

	auto begin() { return begin_; }
	auto end() { return end_; }

	void set_begin(const std::string& ch) { begin_ = ch; }
	void set_end(const std::string& ch) { end_ = ch; }

private:
	std::string begin_;
	std::string end_;
	std::string content_; //this field is useful to debug the workflow of parsing
	int level_;
};
