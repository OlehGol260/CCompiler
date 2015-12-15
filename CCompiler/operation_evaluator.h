#pragma once
#include <functional>
#include <memory>
#include "variable.h"
#include "lexeme.h"
#include <vector>

class OperationEvaluatorInterface
{
public:
	virtual ~OperationEvaluatorInterface() = default;
	virtual std::string SupportedOperation() = 0;
	virtual std::shared_ptr<Variable> Evaluate(const std::vector<std::shared_ptr<Variable>>&) = 0;
	virtual std::vector<VariableType> SupportedTypes() = 0;
};

class OperationEvaluator : public OperationEvaluatorInterface
{
public:
	using Action = std::function<std::shared_ptr<Variable>(const std::vector<std::shared_ptr<Variable>>&)>;

	OperationEvaluator(const std::string& operation, const std::vector<VariableType>& var_types, Action action) :
		m_operation_(operation), m_action_(action), m_variable_types_(var_types) {}

	virtual ~OperationEvaluator() = default;

	virtual std::shared_ptr<Variable> Evaluate(const std::vector<std::shared_ptr<Variable>>& operands) override
	{
		return m_action_(operands);
	}
	virtual std::string SupportedOperation() override
	{
		return m_operation_;
	};
	virtual std::vector<VariableType> SupportedTypes() override
	{
		return m_variable_types_;
	}
private:
	const std::string m_operation_;
	const Action m_action_;
	const std::vector<VariableType> m_variable_types_;
};
