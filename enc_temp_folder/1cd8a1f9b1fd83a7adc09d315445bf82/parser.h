#pragma once
#include "lexem_interface.h"
#include <vector>
#include <memory>
#include "context.h"
#include "typedefs.h"

class Parser
{
private:

public:
	//all available lexems passed here
	void Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems);

private:
	void ParseComplex(const std::vector< std::shared_ptr<LexemInterface>>& lexems);

	static bool IsVariableValid(const std::string& var_name);
private:

	std::shared_ptr<Context> main_context_;
};
