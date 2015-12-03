#include "context_parser.h"

//int ContextParser::parse(char open, char close, int current_index, int current_level)
//{
//	int length = code_.length();
//	char current_char;
//	auto current_context = std::make_shared<Context>(current_level);
//	current_context->set_open_pos(current_index);
//
//	contexts_.push_back(current_context);
//
//	for (auto i = current_index; i < length; ++i)
//	{
//		current_char = code_.at(i);
//		if (current_char == open)
//		{
//			i++;
//			current_level++;
//			i = parse(open, close, i, current_level);
//			current_level--;
//			continue;
//		}
//		if (current_char == close)
//		{
//			if (current_context->level() != 0)
//			{
//				current_context->set_open(open);
//				current_context->set_close(close);
//				current_context->set_end_pos(i);
//				return i;
//			}
//		}
//		current_context->append(current_char);
//	}
//
//	return 0;
//}
