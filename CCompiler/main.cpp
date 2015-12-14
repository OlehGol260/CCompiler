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
	Compiler compiler;
	compiler.Compile(" int value = 2; int power = 4; int result = 1; for(int i = 0; i < power; i = i + 1 ) { result =result * value; } print(result);");

	return 0;
}


