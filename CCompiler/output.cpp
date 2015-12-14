#include "output.h"

void Output::VariableDeclaration(VariableType vt, std::shared_ptr<Variable> v)
{
	std::stringstream ss;
	std::string type;
	switch(vt)
	{
	case VariableType::kInt:
		type = "int";
		break;
	case VariableType::kBool:
		type = "bool"; 
		break;
	case VariableType::kFloat:
		type = "float";
		break;
	default: break;
	}
	ss_ << "\nVariable '" << v->name() << "' of type " << type;
}

std::string Output::Str() const
{
	return ss_.str();
}

void Output::PrintStatement(PrintType pt, const std::string& message)
{
	std::string msg = "\nConsole out: ";
	msg += pt == PrintType::kVariable ? "value of variable " : "";
	ss_ << msg << message;
}
