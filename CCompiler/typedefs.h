#pragma once

#include <memory>
#include <vector>

#include "lexem_interface.h"

using lexems_interface_vec = std::vector<std::shared_ptr<LexemInterface>>;
using lexem_interfaces_iter = lexems_interface_vec::const_iterator;
using lexem_interfaces_reverse_iter = lexems_interface_vec::const_reverse_iterator;

using LT = LexemType;