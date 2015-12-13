#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "operation_evaluater.h"
#include "lexeme.h"
#include "err_msg.h"
class Operations
{
public:
	void Register(std::shared_ptr<OperationEvaluatorInterface> oei)
	{
		m_operations_.push_back(oei);
	}
	std::shared_ptr<Variable> Evaluate(const std::string& operation, const std::vector < std::shared_ptr<Variable>>& vars) const
	{
		std::vector<VariableType> var_types;
		var_types.reserve(vars.size());
		for (auto p_var : vars) { var_types.push_back(p_var->type()); }
		auto found = std::find_if(m_operations_.cbegin(), m_operations_.cend(), [&](std::shared_ptr<OperationEvaluatorInterface> oei)
		{
			return oei->SupportedOperation() == operation && oei->SupportedTypes() == var_types;
		});
		if (found == m_operations_.cend())
		{
			ErrMessage::AbortUnhandledOperation(operation, var_types);
		}
		return (*found)->Evaluate(vars);
	}
private:
	std::vector<std::shared_ptr<OperationEvaluatorInterface>> m_operations_;
};
