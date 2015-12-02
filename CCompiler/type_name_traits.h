#pragma once

#include <typeinfo>
#include <string>

template <typename T>
struct TypeName
{
	static std::string Get()
	{
		return typeid(T).name();
	}
};

template <>
struct TypeName<int>
{
	static std::string Get()
	{
		return "int";
	}
};

template <>
struct TypeName<float>
{
	static std::string Get()
	{
		return "float";
	}
};