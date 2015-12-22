#pragma once
#include <map>

#include "typedefs.h"

class Obfuscator
{
public:
	Obfuscator() = default;
	~Obfuscator() = default;
	void Obfuscate(const Statements& statements);
	
private:
	void ObfuscateBlock(const Statements& statements, int& count);
	void ObfuscateRecursive(std::shared_ptr<LexemeInterface> li, int& count);
	void ObfuscateVariable(std::shared_ptr<LexemeInterface> li, int& count);
	void ObfuscateIfElse(std::shared_ptr<LexemeInterface>, int& count);
	void ObfuscateInsideFunction(std::shared_ptr<LexemeInterface>li, int& count);
	void ObfuscateInsideLoop(std::shared_ptr<LexemeInterface>li, int& count);

	std::string GetObfuscated(const std::string& var) const;
	bool IsVarHasObfusc(const std::string& var) const;
	std::string GenerateObfuscated(const std::string& previous_var_name, int& count);
private:
	std::map<std::string, std::string> m_var_to_obfusc_; /*!< key - variable as it is, value - obfuscated variable*/
	static std::string kObfuscatedNameBase;
};
