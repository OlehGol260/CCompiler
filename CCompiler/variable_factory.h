#pragma once
#include <memory>
#include <assert.h>

#include "variable.h"
#include "variable_int.h"
#include "variable_float.h"
#include "variable_bool.h"


class VariableFactory
{
public:
	static std::shared_ptr<Variable> Generate(const std::string& type, const std::string& name, const std::string& value)
	{
		assert(!value.empty() && !name.empty() && "Tried to generate empty variable");

		if (type == "int") {
			return std::make_shared<VariableInt>( name, value);
		} 
		if (type == "bool") {
			return std::make_shared<VariableBool>(name, value);
		}
		if (type == "float") {
			return std::make_shared<VariableFloat>(name, value);
		}
		return nullptr;
	}
	static std::shared_ptr<Variable> GenerateAnonym(VariableType type, const std::string& value)
	{
		assert(!value.empty() && "Tried to generate empty variable");
		switch(type)
		{
		case VariableType::kInt: 
			return std::make_shared<VariableInt>("", value); 
		case VariableType::kBool:
			return std::make_shared<VariableBool>("", value); 
		case VariableType::kFloat:
			return std::make_shared<VariableFloat>("", value);
		default: break;
		}
		return nullptr;
	}
	static std::shared_ptr<Variable> Generate(const std::string& type, const std::string& name)
	{
		assert(!name.empty() && "Tried to generate empty variable");

		if (type == "int") {
			return std::make_shared<VariableInt>(name);
		}
		if (type == "bool") {
			return std::make_shared<VariableBool>(name);
		}
		if (type == "float") {
			return std::make_shared<VariableFloat>(name);
		}
		return nullptr;
	}
};
