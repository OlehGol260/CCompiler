#pragma once
#include "variable.h"

class VariableBool : public Variable
{
public:
	virtual ~VariableBool() = default;

	explicit VariableBool(bool v) : VariableBool("", v) {}
	explicit VariableBool(const std::string& name) : Variable(VariableType::kBool, name), m_value_(false) {}

	VariableBool(const std::string& name, bool value) : Variable(VariableType::kBool, name), m_value_(value) {}

	VariableBool(const std::string& name, const std::string& value) : VariableBool(name)
	{
		m_value_ = value == "true";
	}

	auto value() const noexcept { return m_value_; }

	void set_value(bool v) { m_value_ = v; }
private:
	bool m_value_;
};
