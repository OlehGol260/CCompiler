#include <iostream>
#include "lexer.h"
#include "utils.h"
int main()
{
	Lexer lx;
	lx.GenerateLexems("int a=0; while(a<10){int b=10;}");
	lx.print();
	//std::cout << Lexer::AddSpaces() << std::endl;
	return 0;
}