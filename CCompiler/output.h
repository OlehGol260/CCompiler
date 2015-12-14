#pragma once
#include <string>
#include <sstream>
#include "type_name_traits.h"

class Output
{
public:
	void VariableDeclaration(VariableType vt, std::shared_ptr<Variable> v);
	std::string Str() const;

	template<typename T>
	void Assignement(std::shared_ptr<Variable> var, T value)
	{
		assert(var);
		ss_ << " Assign " << value << " to variable '" << var->name() << "'";
	}

	void PrintStatement(PrintType pt, const std::string& message);
private:
	std::stringstream ss_;
};
