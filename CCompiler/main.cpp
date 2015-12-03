#include <iostream>
#include "lexer.h"
#include "utils.h"
#include "parser.h"
int main()
{
	Lexer lx;
	lx.GenerateLexems("int a=0; while(a<10){int b=a_;}");
	lx.print();
	std::cout << "\n\n\n" << std::endl;
	Parser p;
	p.Parse(lx.lexems());
	//std::cout << Lexer::AddSpaces() << std::endl;
	return 0;
}