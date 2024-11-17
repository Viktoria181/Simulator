#include "Circuit.h"
#include<iostream>
#pragma warning(disable: 4996)

Circuit::Circuit(const char* name, const char* variables, const char* expression)
{
	this->name = (char*)malloc(7 * sizeof(char));
	if (this->name != nullptr) {
		strcpy(this->name, name);
	}
	else {
		this->name = nullptr;
		throw std::runtime_error("Memory allocation failed for 'name'");
		//std::cerr << "Memory allocation failed for 'name'. Object is in an invalid state." << std::endl;
	}

	// Allocate memory for 'variables' and check for successful allocation
	this->variables = (char*)malloc(7 * sizeof(char));
	if (this->variables != nullptr) {
		strcpy(this->variables, variables);
	}
	else {
		this->variables = nullptr;
		throw std::runtime_error("Memory allocation failed for 'variables'");
		// std::cerr << "Memory allocation failed for 'name'. Object is in an invalid state." << std::endl;
	}

	// Allocate memory for 'expression' and check for successful allocation
	this->expression = (char*)malloc(7 * sizeof(char));
	if (this->expression != nullptr) {
		strcpy(this->expression, expression);
	}
	else {
		this->expression = nullptr;
		throw std::runtime_error("Memory allocation failed for 'expression'");
		//std::cerr << "Memory allocation failed for 'name'. Object is in an invalid state." << std::endl;
	}
}
Circuit::~Circuit()
{
	free(name);
	free(variables);
	free(expression);
}

const char* Circuit::getName() const
{
	return name;
}
const char* Circuit::getExpression() const
{
	return expression;
}
const char* Circuit::getVariables() const
{
	return variables;
}

// Rec function
int Circuit::evaluate(const char* expr, const char* values, int& pos)
{
	if (expr[pos] == '\0')
	{
		std::cerr << "ERROR!" << std::endl;
		return 0;
	}

	char current = expr[pos++];

	if (current == '&')
	{
		int result = 1;
		pos += 1;

		while (expr[pos] != ')') {
			result &= evaluate(expr, values, pos);
			if (expr[pos] == ',') pos++;
		}
		pos++;
		return result;
	}
	else if (current == '|') {
		int result = 0;
		pos += 1;

		while (expr[pos] != ')') {
			result |= evaluate(expr, values, pos);
			if (expr[pos] == ',')
				pos++;
		}
		pos++;
		return result;
	}
	else if (current == '!') {
		int result = 0;
		pos += 1;

		while (expr[pos] != ')') {
			int temp = evaluate(expr, values, pos);
			if (temp == 0)
				result = 1;
			else
				result = 0;
			if (expr[pos] == ',')
				pos++;
		}
		pos++;
		return result;
	}
	else if (std::isalpha(current))
	{
		int index = current - 'a';

		return values[index] == '1' ? 1 : 0;
	}

	std::cerr << "ERROR!" << std::endl;
	return 0;
}
