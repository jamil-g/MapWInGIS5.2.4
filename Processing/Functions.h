#pragma once
#include "CustomExpression.h"

namespace parser
{
	extern std::vector<CustomFunction*> functions;
	void InitializeFunctions();
	CustomFunction* GetFunction(CStringW name);
	void ReleaseFunctions();
};


