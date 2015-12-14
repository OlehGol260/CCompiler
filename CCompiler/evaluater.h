﻿#pragma once
#include <vector>
#include <memory>
#include "variable.h"

#include "context.h"
#include "variable_factory.h"
#include "operations.h"
#include "output.h"
class Evaluater
{
public:
	Evaluater() { this->AddOperations(); }

	~Evaluater() = default;

	void Print() const;
	void Evaluate(const std::vector<std::shared_ptr<Statement>>& roots);
	void Clear();
private:
	void AddVariable(std::shared_ptr<Variable> var, std::vector<std::shared_ptr<Variable>>& lcl_vars);
	void ClearOutOfScopeVars(const std::vector<std::shared_ptr<Variable>>& lcl_vars);

	std::shared_ptr<Variable> EvaluateSqrt(std::shared_ptr<LexemeInterface> root) const;
	std::shared_ptr<Variable> FindVariableByName(const std::string& var_name) const;
	void EvaluateAssignment(std::shared_ptr<LexemeInterface> assignment_st) ;
	std::shared_ptr<Variable> EvaluateVarDecl(std::shared_ptr<LexemeInterface> assignment_st) const;
	void EvaluateIf(std::shared_ptr<LexemeInterface> if_st);

	std::shared_ptr<Variable> EvaluateExpression(std::shared_ptr<LexemeInterface> root) const;

	void EvaluateForLoop(std::shared_ptr<LexemeInterface> root);
	void EvaluateWhileLoop(std::shared_ptr<LexemeInterface> root);

	
	template<class T>
	void SetAndEvaluate(std::shared_ptr<Variable> var, std::shared_ptr<LexemeInterface> lexem)
	{
		auto evaluated_result = EvaluateExpression(lexem);
		auto dest_value = std::static_pointer_cast<T>(var);
		switch(evaluated_result->type())
		{
		case VariableType::kInt:
		{
			auto int_result = std::static_pointer_cast<VariableInt>(evaluated_result);
			m_output_.Assignement(var, int_result->value());
			dest_value->set_value(int_result->value());
		}
		break;
		case VariableType::kFloat:
		{
			auto float_result = std::static_pointer_cast<VariableFloat>(evaluated_result);
			m_output_.Assignement(var, float_result->value());
			dest_value->set_value(float_result->value());
		}
		break;
		case VariableType::kBool:
		{
			auto bool_result = std::static_pointer_cast<VariableBool>(evaluated_result);
			m_output_.Assignement(var, bool_result->value());
			dest_value->set_value(bool_result->value());
		}
		break;
		}
	}

	void EvaluatePrint(std::shared_ptr<LexemeInterface> print_st);
	void EvaluateBlock(const std::vector<std::shared_ptr<Statement>>& roots);
	
	void AddOperations();
private:
	std::vector<std::shared_ptr<Variable>> m_vars;
	Operations m_operations_;
	Output m_output_;
};
