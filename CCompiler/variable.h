#pragma once
#include "enums.h"

/*!
	Base class for variables. Holds variable type and its name.
*/
class Variable
{
public:
	virtual ~Variable() = default;
	Variable(VariableType var_type, const std::string& var_name) :m_var_type_(var_type),  m_name_(var_name) {}

	auto type() const noexcept { return m_var_type_; }
	auto name() const noexcept { return m_name_; }
private:
	const VariableType m_var_type_; /*!< variable type*/
	const std::string m_name_; /*!< variable name*/
};




