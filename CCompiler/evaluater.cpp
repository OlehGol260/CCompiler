#include "evaluater.h"

#include <algorithm>
#include <assert.h>
#include "typedefs.h"
#include "lexeme_condition.h"
#include "variable_factory.h"
#include "err_msg.h"


void Evaluater::Evaluate(const std::vector<std::shared_ptr<Statement>>& root_statements)
{
	EvaluateBlock(root_statements);
}

void Evaluater::AddVariable(std::shared_ptr<Variable> var, std::vector<std::shared_ptr<Variable>>& lcl_vars)
{
	assert(var && "Try to add empty variable");
	m_vars.push_back(var);
	lcl_vars.push_back(var);
}

void Evaluater::ClearOutOfScopeVars(const std::vector<std::shared_ptr<Variable>>& lcl_vars)
{
	for (auto lcl_it = lcl_vars.begin(); lcl_it != lcl_vars.end(); ++lcl_it)
	{
		m_vars.erase(remove(m_vars.begin(), m_vars.end(), *lcl_it)); //erase-remove idiom
	}
}

std::shared_ptr<Variable> Evaluater::FindVariableByName(const std::string& var_name) const
{
	auto it = std::find_if(m_vars.cbegin(), m_vars.cend(), [var_name](std::shared_ptr<Variable> var)
	{
		return var_name == var->name();
	});
	return it != m_vars.cend() ? *it : nullptr;
}

void Evaluater::EvaluateAssignment(std::shared_ptr<LexemeInterface> assignment_root) const
{
	assert(assignment_root && "Empty assignment");
	auto root_lexem = std::static_pointer_cast<Lexeme>(assignment_root);
	auto variable_name = root_lexem->left()->value();
	auto var = FindVariableByName(variable_name);
	if (!var) { ErrMessage::AbortUndeclaredVariable(variable_name); }

	auto var_type = var->type();
	switch (var_type)
	{
	case VariableType::kInt:
		SetAndEvaluate<VariableInt, int>(var, root_lexem);
		break;
	case VariableType::kBool:
		SetAndEvaluate<VariableBool, bool>(var, root_lexem);
		break;
	case VariableType::kFloat:
		SetAndEvaluate<VariableFloat, float>(var, root_lexem);
		break;
	}
}

std::shared_ptr<Variable> Evaluater::EvaluateVarDecl(std::shared_ptr<LexemeInterface> var_decl) const
{
	assert(var_decl && "Empty var declaration");
	auto var_decl_lexeme = std::static_pointer_cast<Lexeme>(var_decl);
	//auto a = VariableFactory::Generate(var_decl_lexeme->left()->value(),  var_decl_lexeme->right()->value());
	return VariableFactory::Generate(var_decl_lexeme->left()->value(), var_decl_lexeme->right()->value());
}

void Evaluater::EvaluateIf(std::shared_ptr<LexemeInterface> if_st)
{
	auto if_lexem = std::static_pointer_cast<LexemeCondition>(if_st);

	//auto condition_result = EvaluateBoolExpression(if_lexem->conidition()->root());
	auto condition_result = true;
	if (condition_result)
	{
		EvaluateBlock(if_lexem->if_body()->roots());
	}
	else if (if_lexem->else_body())
	{
		EvaluateBlock(if_lexem->else_body()->roots());
	}
}

std::shared_ptr<Variable> Evaluater::InnerEvaluate(std::shared_ptr<LexemeInterface> root) const
{
	
	return 0;
}

void Evaluater::EvaluateBlock(const std::vector<std::shared_ptr<Statement>>& root_statements)
{
	assert(!root_statements.empty() && "There is nothing to evaluate");
	std::vector<std::shared_ptr<Variable>> variables_in_current_scope;
	std::shared_ptr<LexemeInterface> root;
	for (auto root_statement : root_statements)
	{
		root = root_statement->root();
		if (root_statement->var_init())
		{
			AddVariable(EvaluateVarDecl(std::static_pointer_cast<Lexeme>(root_statement->var_init())), variables_in_current_scope);
		}
		switch (root->type())
		{
		case LT::kIf:
			EvaluateIf(root);
			break;
		case LT::kAssignment:
			EvaluateAssignment(root);
			break;
		}
	}
	ClearOutOfScopeVars(variables_in_current_scope);
}

void Evaluater::AddOperations()
{
	// INT + INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("+",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableInt>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			+
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT + FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("+",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			+
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT + INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("+",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			+
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT + FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("+",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			+
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	//////////********************************************//////////////////

	// INT - INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("-",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableInt>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			-
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT - FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("-",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			-
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT - INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("-",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			-
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT - FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("-",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			-
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	//////////********************************************//////////////////

	// INT * INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("*",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableInt>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			*
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT * FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("*",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			*
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT * INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("*",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			-
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT * FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("*",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			*
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	//////////********************************************//////////////////

	// INT / INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("/",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableInt>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			/
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT / FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("/",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			/
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT / INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("/",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			/
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT / FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("/",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			/
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
}
