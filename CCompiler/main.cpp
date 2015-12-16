#include "compiler.h"

int main()
{
	Compiler compiler;
	compiler.Compile("E://myfile.txt",
		" bool bool_ = (!(10 > 3 || 3 == 0));\
print(bool_);\
       bool pnh = !true;\
if (!pnh) {\
print(\"pnh\");\
}\
		int a = 4;\
		 int b = 21;\
		 int c = 5;\
		 float x1 = 0;\
		 float x2 = 0;\
		 int D = b * b - 4 * a * c; \
		 if (!(D > 0 || D == 0))\
		 {\
			 x1 = ((0 - b) + sqrt(D)) / (2 * a);\
			 x2 = ((0 - b) - sqrt(D)) / (2 * a);\
			 print(x1);\
			 print(x2);\
		 } \
		else\
		{\
			print(\"Descriminant must be greater or equal to zero\");\
		}");
	//Test for loop
	//compiler.Compile("E://myfile.txt", "int a = 0; for (int i = 0; true; i = i + 1;) {a = a + 1;} print(a);");
//	compiler.Compile("E://myfile.txt", "int a = 0; while(true) {a = a + 1;} print(a);");
	return 0;
}


