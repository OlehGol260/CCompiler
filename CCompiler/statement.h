#pragma once

#include <memory>
#include "lexem_interface.h"
#include "lexem.h"
class Statement
{
public:
	Statement() : Statement(nullptr) {}
	explicit Statement(std::shared_ptr<LexemInterface> lexem) : root_(lexem) {}
	~Statement() = default;


	auto root() { return root_; }
private:
	std::shared_ptr<LexemInterface> root_;

	//workaround to work with variable initialization
	//this var will hold tree of type
	//	left node - type lexem
	//	right node - var lexem
	std::shared_ptr<Lexem> init;
};
