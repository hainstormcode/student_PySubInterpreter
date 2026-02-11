#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "lexAnalysis.h"
#include "expevaluator.h"
#include <string>
using namespace std;


class Interpreter
{
private:

public:
    Expevaluator symbol;
    
    void run(lexicalAnalyser::tokenType);
    
    
    void interpretLine(lexicalAnalyser::tokenType& allCode, int& i);
    
    
    void doPrint(lexicalAnalyser::tokenLineType, int, Expevaluator&);
    void assignVariables(string, string);
    bool conditionChecker(string conditional);
    
    
    lexicalAnalyser::tokenType extractBlock(lexicalAnalyser::tokenType& allCode, int& currentLineIndex);
    
    
    void executeBlock(lexicalAnalyser::tokenType& block);

    void runWhile(string conditional, lexicalAnalyser::tokenType& block);
    void runFor(string iteratorVar, int start, int end, int step, lexicalAnalyser::tokenType& block);

    //variables
    bool whileGoing = false;
    bool whileDone = false;
    bool ifTrue = false;
    bool sudo = false;
    string conditional = "";
    int ifI = 0;
};
#endif
