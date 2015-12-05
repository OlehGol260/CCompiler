#include <iostream>
#include "lexer.h"
#include "utils.h"
#include "parser.h"
int main()
{
	Lexer lx;
	//int a=0; while(a<10){int b=a_;}
	//lx.GenerateLexems("a = 12*3+2;");
	//lx.GenerateLexems("int a = (10/5)*(3+2)<(5%10);");
	lx.GenerateLexems("bool a = (125 < 10 + 15) && (15 == 10);");
	lx.print();
	std::cout << "\n\n\n" << std::endl;
	Parser p;
	p.Parse(lx.lexems());
	//std::cout << Lexer::AddSpaces() << std::endl;
	return 0;
}