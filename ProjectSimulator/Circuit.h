#pragma once
#pragma warning(disable: 4996)

const int BUFFER = 1024;

class Circuit
{
	char* name = nullptr;
	char* variables = nullptr;
	char* expression = nullptr;
public:
	Circuit(const char* name, const char* variables, const char* expression);
	~Circuit();

	const char* getName() const;
	const char* getExpression() const;
	const char* getVariables() const;

	int evaluate(const char* expression, const char* arrOfVariables, int&);
};

