#pragma once

#include <vector>
#include <memory>
#include "operation_evaluater.h"
#include "lexeme.h"

class Operations
{
public:
	void Register(std::shared_ptr<OperationEvaluatorInterface> oei)
	{
		m_operations_.push_back(oei);
	}
private:
	std::vector<std::shared_ptr<OperationEvaluatorInterface>> m_operations_;
};
