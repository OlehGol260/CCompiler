#include "compiler.h"

int main()
{

	Compiler compiler;
	compiler.Compile("E://myfile.txt", "int a = 4; int b = 21; int c = 5; float x1 = 0; float x2 = 0; int D = b * b - 4 * a * c; \
						if (D > 0 || D == 0) { x1 = ((0 - b) + sqrt(D)) / (2 * a);  x2 = ((0 - b) - sqrt(D)) / (2 * a); print(x1); print(x2); } \
						else { print(\"Descriminant must be greater or equal to zero\"); }");

		return 0;
}


