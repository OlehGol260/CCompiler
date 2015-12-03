#include <iostream>
#include "lexer.h"
#include "utils.h"
int main()
{
	Lexer lx;
	lx.GenerateLexems("int a=0; while(a<10){int b=10;}");
	lx.print();
	auto a = std::make_shared<LexemInterface>(LexemType::kLiteral, "10");
	auto b = std::make_shared<LexemInterface>(LexemType::kLiteral, "11");
	std::vector<std::shared_ptr<LexemInterface>> v;
	v.push_back(a);
	v.push_back(b);
	v.clear();
	if (!a)
	{
		std::cout << "PIZDEC" << std::endl;
	} else
	{
		std::cout << a->value() << std::endl;
	}
	//std::cout << Lexer::AddSpaces() << std::endl;
	return 0;
}