#pragma once

#include <memory>
#include <vector>

#include "lexeme_interface.h"

using lexem_interface_vec = std::vector<std::shared_ptr<LexemeInterface>>;
using lexeme_interfaces_iter = lexem_interface_vec::const_iterator;
using lexeme_interfaces_reverse_iter = lexem_interface_vec::const_reverse_iterator;

using LT = LexemeType;