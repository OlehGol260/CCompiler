#pragma once

#include "variable.h"

class VariableFloat : public Variable
{
public:
	virtual ~VariableFloat() = default;
	explicit VariableFloat(float v) : VariableFloat("", v) {}
	explicit VariableFloat(const std::string& name) : Variable(VariableType::kFloat, name), m_value_(0.0f) {}
	VariableFloat(const std::string& name, float value) : Variable(VariableType::kFloat, name), m_value_(value) {}

	VariableFloat( const std::string& name, const std::string& value) : VariableFloat(name)
	{
		m_value_ = static_cast<float>(stof(value));
	}
	auto value() const noexcept { return m_value_; }
	void set_value(float v) { m_value_ = v; }
private:
	float m_value_;
};