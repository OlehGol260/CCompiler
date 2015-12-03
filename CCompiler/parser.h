﻿#pragma once
#include "lexem_interface.h"
#include <vector>
#include <memory>
#include "context.h"

class Parser
{
private:
	using lexems_interface_vec = std::vector<std::shared_ptr<LexemInterface>>;
public:
	//all available lexems passed here
	void Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems);

	
private:
	void ParseStatement(const std::vector< std::shared_ptr<LexemInterface>>& lexems);
	void ParseComplex(const std::vector< std::shared_ptr<LexemInterface>>& lexems);

	static bool IsVariableValid(const std::string& var_name);
private:

	std::shared_ptr<Context> main_context_;
};