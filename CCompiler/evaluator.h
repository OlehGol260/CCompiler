#pragma once
#include <vector>
#include <memory>

#include "variable.h"
#include "context.h"
#include "variable_factory.h"
#include "operations.h"

/*!
	This class evaluates an original code passed as vector of statements
*/
class Evaluator
{
public:
	Evaluator() { this->AddOperations(); }

	~Evaluator() = default;

	/*!
		Evaluates the block of statements
		\param roots statements to be evaluated
	*/
	void Evaluate(const std::vector<std::shared_ptr<Statement>>& roots);
private:
	/*!
		Adds variable var to both lcl_vars and m_vars_. Lcl_vars are used then to correctly clear
		variables which leaves a scope where they have been created
		\param var var which has to be added to both vectors
		\param lcl_vars variables declared in a current scope
	*/
	void AddVariable(std::shared_ptr<Variable> var, std::vector<std::shared_ptr<Variable>>& lcl_vars);

	/*!
		Clears variable which leaves a current scope(scope of a curly braces)
	*/
	void ClearOutOfScopeVars(const std::vector<std::shared_ptr<Variable>>& lcl_vars);

	/*!
		Evaluate sqrt function
		\param root syntax tree for sqrt 
		\return variable holding VariableFloat
	*/
	std::shared_ptr<Variable> EvaluateSqrt(std::shared_ptr<LexemeInterface> root) const;


	std::shared_ptr<Variable> EvaluateLogicalNot(std::shared_ptr<LexemeInterface> root) const;
	std::shared_ptr<Variable> EvaluateUnaryOperation(std::shared_ptr<LexemeInterface> root) const;

	/*!
		Find a variable by given name in m_vars
		\param var_name what to find
		\return valid ptr if found, nullptr if such a variable doesn't exist
	*/
	std::shared_ptr<Variable> FindVariableByName(const std::string& var_name) const;

	/*!
		Evaluates assignment statement and set the result to the corresponding variable
		If variable doesn't exist, the method terminates the program
		\param assignment_st root of assignment syntax tree
	*/
	void EvaluateAssignment(std::shared_ptr<LexemeInterface> assignment_st) const;

	/*!
		Evaluates variable declaration and return newly created variable
		\param assignment_st lexeme with nodes: left - var type, right - var name
		\return newly created variable
	*/
	std::shared_ptr<Variable> EvaluateVarDecl(std::shared_ptr<LexemeInterface> assignment_st) const;

	/*!
		Evaluates if/else statement
		\param if_st root node of if/else syntax tree
	*/
	void EvaluateIfElse(std::shared_ptr<LexemeInterface> if_st);

	/*!
		Evaluates expression passing as syntax tree and return result as anonym variable.
		This function works recursively processing a binary syntax tree using inorder traversal
		\param root root of a syntax tree of an expression
		\return anonymous variable of the result
	*/
	std::shared_ptr<Variable> EvaluateExpression(std::shared_ptr<LexemeInterface> root) const;

	/*!
		Evaluates for loop. For loop must have 3-statement condition. Fails otherwise.
		\param root syntax tree of for loop
	*/
	void EvaluateForLoop(std::shared_ptr<LexemeInterface> root);

	/*!
		Evaluates while loop
		\param root syntax tree of while loop
	*/
	void EvaluateWhileLoop(std::shared_ptr<LexemeInterface> root);

	/*!
		Evaluate an expression and set the result to a specified variable
		\param var the result of expression evaluation will be set to it
		\param lexem roto of expression syntax tree
	*/
	template<class T>
	void SetAndEvaluate(std::shared_ptr<Variable> var, std::shared_ptr<LexemeInterface> lexem) const
	{
		auto evaluated_result = EvaluateExpression(lexem);
		auto dest_value = std::static_pointer_cast<T>(var);
		switch(evaluated_result->type())
		{
		case VariableType::kInt:
		{
			auto int_result = std::static_pointer_cast<VariableInt>(evaluated_result);
			dest_value->set_value(int_result->value());
		}
		break;
		case VariableType::kFloat:
		{
			auto float_result = std::static_pointer_cast<VariableFloat>(evaluated_result);
			dest_value->set_value(float_result->value());
		}
		break;
		case VariableType::kBool:
		{
			auto bool_result = std::static_pointer_cast<VariableBool>(evaluated_result);
			dest_value->set_value(bool_result->value());
		}
		break;
		}
	}
	
	/*!
		Evaluates a print statement
		\param print_st root of a print syntax tree
	*/
	void EvaluatePrint(std::shared_ptr<LexemeInterface> print_st);

	/*!
		Evaluate block of statements
		\param roots statements to be evaluated
	*/
	void EvaluateBlock(const std::vector<std::shared_ptr<Statement>>& roots);
	
	/*!
		Add operation which can be evaluate over given types and given operation sign
	*/
	void AddOperations();
private:
	std::vector<std::shared_ptr<Variable>> m_vars; /*!< all available variables during the code execution*/
	Operations m_operations_; /*!< operations which can be performed over given types and given operation sign*/
	static int m_kInfinite_;
};
