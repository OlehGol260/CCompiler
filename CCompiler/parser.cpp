#include "parser.h"

using State = LexemType;
//outer_state = current_lexem_type == LexemType::kReservedWord ? State::kReservedWordPart : State::kSimplePart;
void Parser::Parse(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
	main_context_ = std::shared_ptr<Context>(0);

	State outer_state = LexemType::kUnknown;
	State inner_state;
	lexems_interface_vec lexems_block;
	//while ()
	for (auto it = lexems.begin(); it != lexems.end(); ++it)
	{
		lexems_block.clear();
		auto current_lexem = *it;
		auto current_lexem_type = current_lexem->type();
		
		switch (outer_state)
		{
		case State::kReservedWord:
		break;
		case State::kMathOp: break;
		case State::kBoolOp: break;
		case State::kPunctuator: break;
		case State::kAssignment: break;
		case State::kType: break;
		case State::kVar: break;
		case State::kLiteral: break;
		case State::kOpenBrace: break;
		case State::kCloseBrace: break;
		case State::kOpenParenthesis: break;
		case State::kCloseParenthesis: break;
		default:
			break;
		}
	}
}


void Parser::ParseStatement(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
}

void Parser::ParseComplex(const std::vector<std::shared_ptr<LexemInterface>>& lexems)
{
}
