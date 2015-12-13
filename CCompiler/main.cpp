#include <iostream>
#include <sstream>
#include <string>
#include "lexer.h"

#include "utils.h"
#include "parser.h"
#include "enums.h"
#include "variable.h"
#include "evaluater.h"
#include "compiler.h"
#include "variable_factory.h"

int main()
{
	
	//
	Compiler compiler;
	compiler.Compile("int a = sqrt(10 + 6) + 4;");
	//compiler.Compile("int a = 0; for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200; for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200;} a = 300;}");
	//compiler.Compile("int a; if (a > 5) { a = 10; } else {a = 15; } ");
//	Lexer lx;
	//int a=0; while(a<10){int b=a_;}
	//lx.GenerateLexems("a = 12*3+2;");
	//lx.GenerateLexems("int a = a * (b + c * (d + e)) * 3 + 1;");
	//lx.GenerateLexems("int a = 0; for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200; for (int i = 0; i < 10; i = i + 1) { a = 100; b = 200;} a = 300;}");
	//lx.GenerateLexems("(a=(b=1));");
	//lx.GenerateLexems("int a = \"int a = 0;\"");
	//lx.GenerateLexems("if (a < 10) { int a = 0;} else { a = 20;}");
	//lx.Parse("if (a > 10) { a = 10; } else { a = 20; }");

	//lx.print();
	//std::cout << "\n\n\n" << std::endl;
	//Parser p;
	//p.Parse(lx.lexems());
	//std::cout << utils::StringToBool("trueragdsg") << std::endl;
	//std::stringstream ss("print(\"Hello World!\"); int a = 100;");
	//std::string d;
	//std::getline(ss, d, ';');
	//std::cout << d << std::endl;
	//std::cout << Lexer::AddSpaces() << std::endl;
	//auto a = std::make_shared<Variable<int>>(10);
	//auto b = std::make_shared<Variable<float>>(20.5);

	//Evaluater ev;
	//ev.Add(a);
	//ev.Add<float>(b);

	return 0;
}


