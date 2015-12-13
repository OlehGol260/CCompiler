#include "evaluater.h"

#include <algorithm>
#include <assert.h>
#include "typedefs.h"
#include "lexeme_condition.h"
#include "variable_factory.h"
#include "err_msg.h"
#include "lexeme_func.h"


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

std::shared_ptr<Variable> Evaluater::EvaluateSqrt(std::shared_ptr<LexemeInterface> root) const
{
	auto root_func = std::static_pointer_cast<LexemeFunc>(root);
	return m_operations_.Evaluate(root->value(), std::vector<std::shared_ptr<Variable>>{EvaluateExpression(root_func->body())});
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

	switch(var->type())
	{
	case VariableType::kInt:
		SetAndEvaluate<VariableInt>(var, root_lexem->right());
		break;
	case VariableType::kFloat:
		SetAndEvaluate<VariableFloat>(var, root_lexem->right());
		break;
	case VariableType::kBool:
		SetAndEvaluate<VariableBool>(var, root_lexem->right());
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

	auto condition_result = EvaluateExpression(if_lexem->conidition()->root());
	assert(condition_result->type() == VariableType::kBool && "Invalid condition");
	auto condition = std::static_pointer_cast<VariableBool>(condition_result)->value();
	if (condition)
	{
		EvaluateBlock(if_lexem->if_body()->roots());
	}
	else if (if_lexem->else_body())
	{
		EvaluateBlock(if_lexem->else_body()->roots());
	}
}

std::shared_ptr<Variable> Evaluater::EvaluateExpression(std::shared_ptr<LexemeInterface> root) const
{
	assert(root && "Tried to evaluate empty expression");
	auto root_lexeme = std::static_pointer_cast<Lexeme>(root);
	auto root_type = root->type();
	auto root_value = root->value();

	if (Grammar::IsBinaryOperator(root_type))
	{
		return m_operations_.Evaluate(
			root_value,
			std::vector<std::shared_ptr<Variable>>{EvaluateExpression(root_lexeme->left()), EvaluateExpression(root_lexeme->right())}
			);
	}
	if (Grammar::IsVariable(root_type))
	{
		auto variable = FindVariableByName(root_value);
		if (!variable)
		{
			ErrMessage::AbortUndeclaredVariable(root_value);
		}
		return variable;
	}
	if (Grammar::IsSqrt(root_type))
	{
		return EvaluateSqrt(root);
	}
	return VariableFactory::GenerateAnonym(Grammar::LexemeTypeToVariableType(root_type), root_value);
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

	//////////********************************************//////////////////

	// INT < INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("<",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			<
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT < FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("<",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			<
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT < INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("<",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			<
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT < FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("<",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			<
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	//////////********************************************//////////////////

	// INT > INT
	m_operations_.Register(std::make_shared<OperationEvaluater>(">",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			>
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT > FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>(">",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			>
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT > INT
	m_operations_.Register(std::make_shared<OperationEvaluater>(">",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			>
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT > FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>(">",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			>
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	//////////********************************************//////////////////

	// INT != INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("!=",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			!=
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT != FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("!=",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			!=
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT != INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("!=",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			!=
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT != FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("!=",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			!=
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	//////////********************************************//////////////////

	// INT == INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("==",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			==
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT == FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("==",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			==
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT == INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("==",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			==
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT == FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("==",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			==
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	//////////********************************************//////////////////

	// BOOL && BOOL
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kBool, VariableType::kBool },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableBool>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableBool>(vars.at(1))->value());
	}));

	// FLOAT && BOOL
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kBool },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableBool>(vars.at(1))->value());
	}));
	// INT && BOOL
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kInt, VariableType::kBool },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableBool>(vars.at(1))->value());
	}));
	// BOOL && FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kBool, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableBool>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	// BOOL && INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kBool, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableBool>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));


	// INT && INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT && FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT && INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT && FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("&&",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			&&
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	//////////********************************************//////////////////

	// BOOL || BOOL
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kBool, VariableType::kBool },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableBool>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableBool>(vars.at(1))->value());
	}));

	// FLOAT || BOOL
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kBool },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableBool>(vars.at(1))->value());
	}));
	// INT || BOOL
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kInt, VariableType::kBool },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableBool>(vars.at(1))->value());
	}));
	// BOOL || FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kBool, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableBool>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));

	// BOOL || INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kBool, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableBool>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));


	// INT || INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kInt, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// INT || FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kInt, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableInt>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));
	// FLOAT || INT
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableInt>(vars.at(1))->value());
	}));
	// FLOAT || FLOAT
	m_operations_.Register(std::make_shared<OperationEvaluater>("||",
		std::vector<VariableType>{VariableType::kFloat, VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableBool>(
			std::static_pointer_cast<VariableFloat>(vars.at(0))->value()
			||
			std::static_pointer_cast<VariableFloat>(vars.at(1))->value());
	}));


	// sqrt(FLOAT)
	m_operations_.Register(std::make_shared<OperationEvaluater>("sqrt",
		std::vector<VariableType>{VariableType::kFloat },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::sqrt(std::static_pointer_cast<VariableFloat>(vars.at(0))->value()));
	}));

	// sqrt(INT)
	m_operations_.Register(std::make_shared<OperationEvaluater>("sqrt",
		std::vector<VariableType>{VariableType::kInt },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::sqrt(static_cast<float>(std::static_pointer_cast<VariableInt>(vars.at(0))->value())));
	}));

	// sqrt(BOOl)
	m_operations_.Register(std::make_shared<OperationEvaluater>("sqrt",
		std::vector<VariableType>{VariableType::kBool },
		[](const std::vector<std::shared_ptr<Variable>>& vars) -> auto {
		return std::make_shared<VariableFloat>(
			std::sqrt(static_cast<float>(std::static_pointer_cast<VariableBool>(vars.at(0))->value())));
	}));
}
