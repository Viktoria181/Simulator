#pragma once
#include "Simulator.h"
#include "Circuit.h"
#include <iostream>

struct TruthTable {
    char** table = nullptr;
    int rows = 0;
    int cols = 0;
};
//
//{
//    Circuit* newOne = (Circuit*)malloc(sizeof(Circuit)); // Заделяме памет за обекта Circuit
//
//    if (newOne != nullptr) { 
//        // Инициализираме обекта с помощта на конструктора
//        newOne = new (newOne) Circuit(name, tempInput, expression); // Извикваме конструктора на Circuit с new placement
//
//        // Добавяме указателя към масива
//        arrOfCircuits[size++] = *newOne;
//    }
class Simulator
{
    Circuit* arrOfCircuits = (Circuit*)malloc(1024 * sizeof(Circuit));
    int size = 0;
public:
    void defineCircuit(const char* name, char* expression);
    bool runCircuit(const char* name, char* arrOfVars); // ARROFVARS ONLY NUMS !!! arrOFVariables = (1, 0, 1)

    void allCircuit(const char* name, const char* expr, int numVariables);
    void findCircuit(TruthTable table);

    Circuit* getCircuit(int index);
    int getIndexOfArr(const char* name);
    bool isCircuitInArr(const char* name);
};
