#pragma once

#include <typeinfo>
#include <string>
#include "variable_factory.h"
template <typename T>
struct TypeName
{
	static std::string Get()
	{
		return typeid(T).name();
	}
};

template <>
struct TypeName<VariableBool>
{
	static std::string Get()
	{
		return "bool";
	}
};

template <>
struct TypeName<VariableInt>
{
	static std::string Get()
	{
		return "int";
	}
};

template <>
struct TypeName<VariableFloat>
{
	static std::string Get()
	{
		return "float";
	}
};