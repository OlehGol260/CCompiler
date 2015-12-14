#include "compiler.h"

int main()
{
	Compiler compiler;
	//compiler.Compile("int value = 2; int power = 4; int result = 1; for(int i = 0; i < power; i = i + 1 ) { result =result * value; } print(result);");
	compiler.Compile("bool a = (10 + 25 *32) > (true || false); if (a) { print(\"OLOLOLOLO\"); } else { a = true; } ");

	return 0;
}


