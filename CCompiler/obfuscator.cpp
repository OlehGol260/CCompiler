#include "obfuscator.h"
#include <assert.h>
#include <string>

#include "grammar.h"
#include "lexeme_condition.h"
#include "lexeme_func.h"
#include "lexeme_loop.h"

std::string Obfuscator::kObfuscatedNameBase = "a";

void Obfuscator::Obfuscate(const Statements& statements)
{
	auto count = 0;
	ObfuscateBlock(statements, count);
}

void Obfuscator::ObfuscateBlock(const Statements& statements, int& count)
{
	for (auto statement = statements.cbegin(); statement != statements.cend(); ++statement)
	{
		auto lexeme = (*statement)->root();
		auto root_type = lexeme->type();
		switch (root_type)
		{
		case LT::kIf:
			ObfuscateIfElse(lexeme, count);
			break;
		case LT::kPrint:
		case LT::kSqrt:
		case LT::kLogicalNot:
			ObfuscateInsideFunction(lexeme, count);
			break;
		case LT::kFor:
		case LT::kWhile:
			ObfuscateInsideLoop(lexeme, count);
			break;
		default:
			ObfuscateRecursive(lexeme, count);
		}
	}
}

void Obfuscator::ObfuscateRecursive(std::shared_ptr<LexemeInterface> li, int& count)
{
	if (!li) { return; }
	auto li_type = li->type();
	if (Grammar::IsBinaryOperator(li_type))
	{
		auto lexeme = std::static_pointer_cast<Lexeme>(li);
		ObfuscateRecursive(lexeme->left(), count);
		ObfuscateRecursive(lexeme->right(), count);
	}
	else if (Grammar::IsLexemeFunc(li_type))
	{
		ObfuscateInsideFunction(li, count);
	}
	else if (Grammar::IsVariable(li_type))
	{
		ObfuscateVariable(li, count);
	}
}

void Obfuscator::ObfuscateVariable(std::shared_ptr<LexemeInterface> li, int& count)
{
	assert(Grammar::IsVariable(li->type()));
	auto var_name = li->value();
	li->set_value(IsVarHasObfusc(var_name) ? GetObfuscated(var_name) : GenerateObfuscated(var_name, count));
}

void Obfuscator::ObfuscateIfElse(std::shared_ptr<LexemeInterface>li, int& count)
{
	assert(li);
	auto condition_lexeme = std::static_pointer_cast<LexemeCondition>(li);
	ObfuscateRecursive(condition_lexeme->condition()->root(), count);
	ObfuscateBlock(condition_lexeme->if_body()->roots(), count);
	if (condition_lexeme->has_else())
	{
		ObfuscateBlock(condition_lexeme->else_body()->roots(), count);
	}
}

void Obfuscator::ObfuscateInsideFunction(std::shared_ptr<LexemeInterface> li, int& count)
{
	assert(li);
	auto func_lexeme = std::static_pointer_cast<LexemeFunc>(li);
	ObfuscateRecursive(func_lexeme->body(), count);
}

void Obfuscator::ObfuscateInsideLoop(std::shared_ptr<LexemeInterface> li, int& count)
{
	assert(li);
	auto loop_lexeme = std::static_pointer_cast<LexemeLoop>(li);
	ObfuscateBlock(loop_lexeme->condition()->roots(), count);
	ObfuscateBlock(loop_lexeme->body()->roots(), count);
}

std::string Obfuscator::GetObfuscated(const std::string& var) const
{
	auto variable = this->m_var_to_obfusc_.find(var);
	assert(variable != m_var_to_obfusc_.cend());
	return variable->second;
}

bool Obfuscator::IsVarHasObfusc(const std::string& var) const
{
	return m_var_to_obfusc_.count(var) > 0;
}

std::string Obfuscator::GenerateObfuscated(const std::string& previous_var_name, int& count)
{
	auto new_var_name = kObfuscatedNameBase + std::to_string(count++);
	m_var_to_obfusc_[previous_var_name] = new_var_name;
	return new_var_name;
}
