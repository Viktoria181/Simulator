#include "Simulator.h"
#include <stack>

bool isInString(const char* string, char symbol)
{
    const char* tempString = string;
    for (int i = 0; i < strlen(string); i++)
    {
        if (symbol == *tempString)
            return true;

        tempString++;
    }
    return false;

}
bool wrongInputHasCommonLetters(char* shorter, char* longer) {
    // This function checks if all characters in `longer` exist in `shorter`.

    size_t lengthOfLonger = strlen(longer);
    size_t length = 0;

    while (length < lengthOfLonger) {
        char* tempShorter = shorter;

        // Check if the current character from `longer` exists in `shorter`
        if (!isInString(tempShorter, *longer)) {
            return false; // If the character is not found, return false
            break; // Optional, since `return` already exits the function
        }

        longer++; // Move to the next character in `longer`
        length++;
    }

    return true; // If all characters from `longer` are found in `shorter`, return true
}
void printString(char* str)
{
    size_t length = strlen(str);
    char* ptr = str;
    for (size_t i = 0; i < length; i++)
        std::cout << ptr[i];
}

bool validateRecursive(const char* expression, int& index, std::stack<char>& negationStack) {
    // Skip leading spaces
    while (expression[index] == ' ')
        ++index;

    // If we reach the end of the string
    if (expression[index] == '\0')
        return negationStack.empty(); // Ensure the negation stack is empty

    char current = expression[index++];

    // Expecting logical operators (&, |, !)
    if (current == '&' || current == '|') {
        if (expression[index] != '(')
            return false; // Missing opening parenthesis after the logical operator

        ++index; // Skip '('

        int subExpressionCount = 0; // Counter for subexpressions inside & or |
        bool expectComma = false;
        while (expression[index] != ')') {
            if (expectComma) {
                if (expression[index] != ',') {
                    return false; // Missing comma
                }
                ++index; // Skip the comma
            }

            if (!validateRecursive(expression, index, negationStack))
                return false;

            ++subExpressionCount; // Increment the counter after a valid subexpression
            expectComma = true; // After the first valid subexpression, expect a comma or closing parenthesis
        }
        ++index; // Skip ')'

        if (subExpressionCount < 2) {
            return false; // Operators & and | must have at least two arguments
        }

        return true;
    }

    if (current == '!') {
        negationStack.push('!'); // Add to the negation stack
        if (expression[index] != '(')
            return false; // Missing opening parenthesis after !

        ++index; // Skip '('

        // Expect exactly one subexpression inside !
        if (!validateRecursive(expression, index, negationStack))
            return false;

        if (expression[index] != ')')
            return false; // Missing closing parenthesis for !

        ++index; // Skip ')'
        negationStack.pop(); // Remove the negation
        return true;
    }

    // If the current character is a letter
    if (std::isalpha(current))
        return true;

    return false; // Invalid character
}
bool isValidExpression(const char* expression) {
    int index = 0;
    std::stack<char> negationStack; // Stack for negations (!)
    return validateRecursive(expression, index, negationStack) && expression[index] == '\0';
}

char* exprOperation(char* expression) // skip (a, b): from the expression and removing spaces
{
    char* ptr = expression;
    while (*ptr != ':')
        ptr++;
    ptr += 2; // Move 2 positions forward to skip `:` and the space after it

    size_t length = std::strlen(ptr);          // Length of original string
    char* result = new char[length + 1];
    int j = 0;                               // Index for the new string

    for (int i = 0; i < length; i++) {
        if (ptr[i] != ' ')            // Copy symbols that are not ' '
            result[j++] = ptr[i];
    }
    result[j] = '\0';
    expression = result;
    return expression;
}
int takeOnlyDigits(char* arr) // take only (1, 0, 1) => 101
{
    int num = 0;
    char* firstTempString = arr;
    char* secondTempstring = firstTempString;
    size_t i = 0, j = 0, length = strlen(arr);

    while (i < length)
    {
        if (*arr == '1' || *arr == '0')
        {
            firstTempString[j++] = *arr;
            num++;
        }
        i++;
        arr++;
    }
    secondTempstring[j] = '\0';
    arr = secondTempstring;
    return num;
}

Circuit* Simulator::getCircuit(int index)
{
    return &arrOfCircuits[index];
}
bool Simulator::isCircuitInArr(const char* name)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(arrOfCircuits[i].getName(), name) == 0)
            return true;
    }
    return false;
}
int Simulator::getIndexOfArr(const char* name)
{
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(arrOfCircuits[i].getName(), name) == 0)
            return index;
        index++;
    }
    return -1;
}

void Simulator::defineCircuit(const char* name, char* expression)
{
    if (isCircuitInArr(name))
    {
        std::cerr << "The integrated circuit " << *name << " already exist! Enter another one! " << std::endl;
        return;
    }

    // check ? a b c : a b c d 
    char* tempInput = (char*)malloc(BUFFER * sizeof(char));
    if (tempInput == nullptr) {
        std::cerr << "Memory allocation for 'command' failed!" << std::endl;
        return;
    }

    char* tempVars = (char*)malloc(BUFFER * sizeof(char));
    if (tempVars == nullptr) {
        std::cerr << "Memory allocation for 'command' failed!" << std::endl;
        return;
    }

    size_t j = 0, k = 0, length = strlen(expression);
    bool tempBool = false;

    // we also should have do we have nums (a, b) as (1, 0, 1)
    const char* tempExpr = expression; // taking the arrays with variables g=from both sides
    for (size_t i = 0; i < length; i++)
    {
        if (*tempExpr == ':')
            tempBool = true;

        if (tempBool == 0 && (*tempExpr >= 'a' && *tempExpr <= 'z'))
            tempInput[j++] = *tempExpr;
        else if (*tempExpr >= 'a' && *tempExpr <= 'z')
            tempVars[k++] = *tempExpr;
        tempExpr++;

        if (i + 1 == length)
        {
            tempInput[j] = '\0';
            tempVars[k] = '\0';
        }
    }

    // a b c -> 1 0 1 0
    if (wrongInputHasCommonLetters(tempInput, tempVars) != true)
    {
        std::cerr << "Wrong input! Some of the variables does not exist! " << std::endl;
    }
    else
    {
        expression = exprOperation(expression); // игнорираме space
        if (!isValidExpression(expression)) // проверка дали въведения израз е валиден
            std::cerr << "Expression is not valid! " << std::endl;
        else {
            Circuit* newOne = new Circuit(name, tempInput, expression); // Създаване на обекта директно с new

            if (newOne != nullptr) {
                // Добавяме указателя към масива
                arrOfCircuits[size++] = *newOne;
                std::cout << "Circuit is defined!" << std::endl;
            }
        }
    }
    free(tempInput);
    free(tempVars);

    tempInput = nullptr;
    tempVars = nullptr;
}
bool Simulator::runCircuit(const char* name, char* arrOfVars)
{
    int isIn = 0, i = 0, numOfDigits = 0;

    for (i = 0; i < size; i++) // isInArr this circuit
    {
        if (strcmp(arrOfCircuits[i].getName(), name) == 0) {
            isIn = true;
            break;
        }
        else {
            isIn = false;
        }
    }
    if (isIn == false)
    {
        std::cerr << "This circuit does not exist! " << std::endl;
        return 0;
    }

    numOfDigits = takeOnlyDigits(arrOfVars); // taking only the digits from arrOFVariables = (1, 0, 1) => 101

    int pos = 0;

    if (strlen(arrOfCircuits[i].getVariables()) != numOfDigits)
    {
        std::cerr << "Wrong input! The number of variables is not equal!" << std::endl;
        return 0;
    }
    int answer = arrOfCircuits[i].evaluate(arrOfCircuits[i].getExpression(), arrOfVars, pos); // &(a, !(c), |(b, a)) and (1,0,1)
    std::cout << "The answer from the operation is: " << answer << "." << std::endl;
    return answer;
}

void Simulator::allCircuit(const char* name, const char* expr, int numVariables) {
    // Generate all combinations from 0 and 1 for n variables
    int numCombinations = std::pow(2, numVariables);

    for (int i = 0; i < numCombinations; ++i) {
        char* values = new char[numVariables + 1];  // plus term zero
        values[numVariables] = '\0';

        // generate variables for current combination
        for (int j = 0; j < numVariables; ++j)
            values[j] = (i >> (numVariables - j - 1)) & 1 ? '1' : '0';  // Извличаме битовете и ги преобразуваме в '0' или '1'

        int pos = 0, result = 0;
        for (int i = 0; i < size; i++)
        {
            if (strcmp(arrOfCircuits[i].getName(), name) == 0)
                result = arrOfCircuits[i].evaluate(expr, values, pos);  // Оценяваме израза за текущата комбинация
        }

        std::cout << "Combination is: ";

        for (int j = 0; j < numVariables; ++j)
            std::cout << values[j] << " | ";

        std::cout << "Result is: " << result << std::endl;
    }
}

void Simulator::findCircuit(TruthTable tableStruct)
{
    char finalString[1024] = ""; // Инициализиране на низ като празен
    int n = tableStruct.cols - 1;

    for (int i = 0; i < tableStruct.rows; ++i) {
        // Нулираме finalString за всяка нова линия
        strcpy(finalString, "");

        if (tableStruct.table[i][n] == '0') {
            std::cout << std::endl;
            std::cout << "Perfect Conjunctive Normal Form (PCNF): ";

            // Начало на конюнкцията
            strcat(finalString, "|(");

            for (int j = 0; j < n; ++j) {
                if (j > 0)
                    strcat(finalString, ","); // Добавяне на запетая между термините

                if (tableStruct.table[i][j] == '1') {
                    strcat(finalString, "!"); // Добавяне на отрицание
                    strcat(finalString, "(");
                    char var[2] = { 'a' + j, '\0' }; // Генерираме променлива 'a', 'b', 'c', ...
                    strcat(finalString, var);
                    strcat(finalString, ")");
                }
                else {
                    char var[2] = { 'a' + j, '\0' }; // Генерираме променлива 'a', 'b', 'c', ...
                    strcat(finalString, var);
                }
            }
            strcat(finalString, ")");
        }
        else if (tableStruct.table[i][n] == '1') {
            std::cout << std::endl;
            std::cout << "Perfect Disjunctive Normal Form (PDNF): ";

            // Начало на дизюнкцията
            strcat(finalString, "&(");

            for (int j = 0; j < n; ++j) {
                if (j > 0)
                    strcat(finalString, ","); // Добавяне на запетая между термините

                if (tableStruct.table[i][j] == '1') {
                    char var[2] = { 'a' + j, '\0' }; // Генерираме променлива 'a', 'b', 'c', ...
                    strcat(finalString, var);
                }
                else {
                    strcat(finalString, "!("); // Добавяне на отрицание
                    char var[2] = { 'a' + j, '\0' }; // Генерираме променлива 'a', 'b', 'c', ...
                    strcat(finalString, var);
                    strcat(finalString, ")");
                }
            }
            strcat(finalString, ")");
        }

        // Извеждаме финалния низ за текущия ред
        std::cout << std::endl;
        printString(finalString); // Предполагам, че printString е дефинирана за извеждане на finalString
    }

    std::cout << std::endl;
}
