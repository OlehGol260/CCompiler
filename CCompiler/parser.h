#pragma once

#include <vector>
#include <memory>

#include "lexem_interface.h"
#include "context.h"

class Parser
{
public:
	Parser() : main_context_(std::make_shared<Context>(0)) {}
	~Parser() = default;

	void Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems);
private:
	static bool IsVariableValid(const std::string& var_name);
private:
	std::shared_ptr<Context> main_context_;
};
