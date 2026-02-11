// Specification file for the DynIntStack class
#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H
#include <map>
#include <iostream>
#include "lexAnalysis.h"

using namespace std;

//if things with lexanalysis stop working, consider removing the tokenClear at the top... might've double cleared on accident


class Expevaluator
{
private:
    typedef map<string, string> symbolTableType;
    static symbolTableType symbolTable;

public:
    // Operations
    int precedence(string);
    int processEquation(string);
    string toPost(vector<pair<string, categoryType>>);
    int evaluate(lexicalAnalyser::tokenLineType postfix);
    int calculate(string, int, int);
    void tableClear() { symbolTable.clear(); }
    void tableAdd(string key, string def) { symbolTable.insert({ key, def }); }
    void displayTable();
    string collectInput(lexicalAnalyser::tokenLineType, int&);
    friend class Interpreter;
};
#endif
//file save test
