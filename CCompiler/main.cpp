#include <iostream>
#include "lexer.h"
#include "utils.h"
#include "parser.h"
int main()
{
	Lexer lx;
	//int a=0; while(a<10){int b=a_;}
	//lx.GenerateLexems("a = 12*3+2;");
	//lx.GenerateLexems("int a = a * (b + c * (d + e)) * 3 + 1;");
	lx.GenerateLexems("for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200; for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200;}}");
	//lx.GenerateLexems("while(a < 100) { int a = 0;}");
	lx.print();
	std::cout << "\n\n\n" << std::endl;
	Parser p;
	p.Parse(lx.lexems());
	//std::cout << Lexer::AddSpaces() << std::endl;
	return 0;
}