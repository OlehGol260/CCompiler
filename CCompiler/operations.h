#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "operation_evaluator.h"
#include "lexeme.h"
#include "err_msg.h"

/*!
	Class holds operations which can be performed over given types and operation
*/
class Operations
{
public:
	/*!	
		Add new operation to m_operations_
		\param oei new operation
	*/
	void Register(std::shared_ptr<OperationEvaluatorInterface> oei)
	{
		m_operations_.push_back(oei);
	}

	/*!
		Evaluates an expression over given types and operation
		If needed operation cannot be found, a corresponding error message will be shown and the program terminates
		\param operation sign of operation
		\param vars vector of variables. It can be one operation(sqrt) or two(any binary operation)
		\return anonymous variable which holds the result of evaluation
	*/
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
	std::vector<std::shared_ptr<OperationEvaluatorInterface>> m_operations_; /*!< operations which can be performed over given types and operation*/
};
