#pragma once
#include <string>

struct Bracket
{
	Bracket(const std::string& open, const std::string& close) : open(open), close(close) {}
	~Bracket() = default;

	bool IsOpen(const std::string& str) const noexcept { return open == str; }
	bool IsClose(const std::string& str) const noexcept { return close == str; }

	std::string open;
	std::string close;
};
