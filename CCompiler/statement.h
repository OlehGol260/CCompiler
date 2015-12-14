#pragma once

#include <memory>
#include "lexeme_interface.h"
#include "lexeme.h"
class Statement
{
public:
	Statement() : Statement(nullptr) {}
	explicit Statement(std::shared_ptr<LexemeInterface> lexem) : root_(lexem), punctuator_(nullptr) {}
	~Statement() = default;


	bool HasPunctuator() const noexcept { return punctuator_ != nullptr; }
	auto punctuator() const noexcept { return punctuator_; }
	void set_punctuator(std::shared_ptr<LexemeInterface> p) { punctuator_ = std::static_pointer_cast<Lexeme>(p); }

	void set_root(std::shared_ptr<LexemeInterface> r) { root_ = r; }
	auto root() { return root_; }

	void set_var_init(std::shared_ptr<Lexeme> i) { var_init_ = i; }
	auto var_init() const noexcept { return var_init_; }

private:
	std::shared_ptr<LexemeInterface> root_;

	std::shared_ptr<Lexeme> punctuator_;
	//workaround to work with variable initialization
	//this var will hold tree of type
	//	left node - type lexem
	//	right node - var lexem
	std::shared_ptr<Lexeme> var_init_;
};
