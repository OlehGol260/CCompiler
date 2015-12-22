#pragma once
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "translator.h"
#include "obfuscator.h"

/*!
	
*/
class Compiler
{
public:
	/*!
		Compile the code and show everything to the standard output
		\param code the origin code to be compiled
	*/
	void Compile(const std::string& code);
	
	/*!
		Compile the code and show evaluation result to the standard output and translator result
		to the specified file
		\param filepath path to the file where english text will be saved
		\param code the origin code to be compiled
	*/
	void Compile(const std::string& filepath, const std::string& code);
	Compiler() = default;

	//Prohibit copying and assigning of objects of this class
	Compiler(const Compiler&) = delete;
	void operator=(const Compiler&) = delete;
private:
	/*!
		Compile the code 
		\param code the origin code to be compiled
	*/
	void InnerCompile(const std::string& code);
private:
	Lexer m_lexer_;
	Parser m_parser_;
	Evaluator m_evaluater_;
	Translator m_translator_;
	Obfuscator m_obfuscator_;
};
