#pragma once

#include <memory>
#include "lexem_interface.h"
#include "lexem.h"
class Statement
{
public:
	Statement() : Statement(nullptr) {}
	explicit Statement(std::shared_ptr<LexemInterface> lexem) : root_(lexem), punctuator_(nullptr) {}
	~Statement() = default;

	void Print() const;

	bool HasPunctuator() const noexcept { return punctuator_ != nullptr; }
	auto punctuator() const noexcept { return punctuator_; }
	void set_punctuator(std::shared_ptr<LexemInterface> p) { punctuator_ = std::static_pointer_cast<Lexem>(p); }

	void set_root(std::shared_ptr<LexemInterface> r) { root_ = r; }
	auto root() { return root_; }

	void set_var_init(std::shared_ptr<Lexem> i) { var_init_ = i; }
	auto var_init() const noexcept { return var_init_; }
private:
	void InnerPrint(std::shared_ptr<LexemInterface> lexem) const;
private:
	std::shared_ptr<LexemInterface> root_;

	std::shared_ptr<Lexem> punctuator_;
	//workaround to work with variable initialization
	//this var will hold tree of type
	//	left node - type lexem
	//	right node - var lexem
	std::shared_ptr<Lexem> var_init_;
};
