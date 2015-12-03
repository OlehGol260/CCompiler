#pragma once
#include "context.h"

class ContextParser
{
public:

	//static std::shared_ptr<Context> GetNextContext


	~ContextParser() = default;
	explicit ContextParser(const std::string& code) : current_level_(0), code_(code) {}
	int parse(char open, char close, int current_index, int current_level);
	auto& contexts() const { return contexts_; }
private:
	int current_level_;
	std::string code_;
	std::vector<std::shared_ptr<Context>> contexts_;
};
