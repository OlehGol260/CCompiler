#pragma once

#include "variable.h"
#include <string>
class VariableInt : public Variable
{
public:
	virtual ~VariableInt() = default;
	
	explicit VariableInt(int value) : VariableInt("", value) {}
	explicit VariableInt(const std::string& name) : VariableInt(name, 0) {}

	VariableInt(const std::string& name, int value) : Variable(VariableType::kInt, name), m_value_(value) {}
	VariableInt(const std::string& name, const std::string& value) : VariableInt(name)
	{
		m_value_ = stoi(value);
	}

	auto value() const noexcept { return m_value_; }
	void set_value(int v) { m_value_ = v; }
private:
	int m_value_;
};