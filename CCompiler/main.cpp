#include <iostream>
#include <sstream>
#include <string>
#include "lexer.h"

#include "utils.h"
#include "parser.h"
int main()
{
	Lexer lx;
	//int a=0; while(a<10){int b=a_;}
	//lx.GenerateLexems("a = 12*3+2;");
	//lx.GenerateLexems("int a = a * (b + c * (d + e)) * 3 + 1;");
	//lx.GenerateLexems("int a = 0; for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200; for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200;} a = 300;}");
	//lx.GenerateLexems("(a=(b=1));");
	//lx.GenerateLexems("int a = \"int a = 0;\"");
	//lx.GenerateLexems("if (a < 10) { int a = 0;} else { a = 20;}");
	lx.GenerateLexems("print();");

	lx.print();
	std::cout << "\n\n\n" << std::endl;
	Parser p;
	p.Parse(lx.lexems());

	//std::stringstream ss("print(\"Hello World!\"); int a = 100;");
	//std::string d;
	//std::getline(ss, d, ';');
	//std::cout << d << std::endl;
	//std::cout << Lexer::AddSpaces() << std::endl;
	return 0;
}