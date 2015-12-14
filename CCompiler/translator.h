#pragma once

#include <memory>
#include <sstream>
#include "context.h"

class Translator
{
public:
	void Translate(std::vector<std::shared_ptr<Statement>> main_context);
	std::string TranslateExpression(std::shared_ptr<LexemeInterface> li);
	void Print() const;
private:
	void TranslateIf(std::shared_ptr<LexemeInterface> li);
	void TranslateVarDeclaration(std::shared_ptr<LexemeInterface> li);
	void TranslatePrint(std::shared_ptr<LexemeInterface> li);
	void TranslateForLoop(std::shared_ptr<LexemeInterface> li);
	void TranslateWhileLoop(std::shared_ptr<LexemeInterface> li);
	void TranslateAssignment(std::shared_ptr<LexemeInterface> li);
	
	void TranslateIfNotEmpty(std::shared_ptr<Context> context, const std::string& msg);
private:
	std::stringstream ss_;
};
