#pragma once

#include <string>
#include <memory>

#include "variable_factory.h"

template<class T>
struct VariableStrTraits
{
	static std::string value(std::shared_ptr<Variable>)
	{
		return "Unhandled type";
	}
};

template<>
struct VariableStrTraits<VariableInt>
{
	static std::string value(std::shared_ptr<Variable> var)
	{
		assert(var);
		return std::to_string(std::static_pointer_cast<VariableInt>(var)->value());
	}
};
template<>
struct VariableStrTraits<VariableFloat>
{
	static std::string value(std::shared_ptr<Variable> var)
	{
		assert(var);
		return std::to_string(std::static_pointer_cast<VariableFloat>(var)->value());
	}
};
template<>
struct VariableStrTraits<VariableBool>
{
	static std::string value(std::shared_ptr<Variable> var)
	{
		assert(var);
		return  std::static_pointer_cast<VariableBool>(var)->value() ? "true" : "false";
	}
};