#include "interpreter.h"
#include "interface.h"
#include "expevaluator.h"
#include "lexAnalysis.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

Interface colors2;
lexicalAnalyser lexAnalysis2;

void Interpreter::run(lexicalAnalyser::tokenType allCode)
{
    colors2.setColor(94);

    ifTrue = false;
    sudo = false;
    whileGoing = false;
    whileDone = false;

    for (int i = 0; i < allCode.size(); i++)
    {
        interpretLine(allCode, i);
    }
}

lexicalAnalyser::tokenType Interpreter::extractBlock(lexicalAnalyser::tokenType& allCode, int& currentLineIndex)
{
    lexicalAnalyser::tokenType block;
    int nextLine = currentLineIndex + 1;
    string baselineIndent = "";

    if (nextLine < allCode.size() && !allCode[nextLine].empty() && allCode[nextLine][0].second == categoryType::INDENT)
    {
        baselineIndent = allCode[nextLine][0].first;
    }
    else
    {
        return block;
    }

    while (nextLine < allCode.size())
    {
        if (allCode[nextLine].empty()) {
            nextLine++;
            continue;
        }

        // Check if line has epic indentation
        if (allCode[nextLine][0].second == categoryType::INDENT)
        {
            string currentIndent = allCode[nextLine][0].first;
            if (currentIndent.length() >= baselineIndent.length() &&
                currentIndent.substr(0, baselineIndent.length()) == baselineIndent)
            {
                lexicalAnalyser::tokenLineType strippedLine = allCode[nextLine];
                
                // Modify the indentation token hehe
                if (currentIndent.length() > baselineIndent.length())
                {
                    strippedLine[0].first = currentIndent.substr(baselineIndent.length());
                }
                else
                {
                    // if indentation matches baseline exactly, remove token completely please
                    strippedLine.erase(strippedLine.begin());
                }
                
                block.push_back(strippedLine);
                nextLine++;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    // Update the main index to skip these lines in the main run loop
    currentLineIndex = nextLine - 1;
    return block;
}

void Interpreter::executeBlock(lexicalAnalyser::tokenType& block)
{
    for (int i = 0; i < block.size(); i++)
    {
        interpretLine(block, i);
    }
}

void Interpreter::interpretLine(lexicalAnalyser::tokenType& allCode, int& i)
{
    if (i >= allCode.size()) return;

    lexicalAnalyser::tokenLineType line = allCode[i];
    int nextBit = 0;

    if (!line.empty())
    {
        if (line[nextBit].second == IDENTIFIER)
        {
            if (line.size() > 1 && line[1].second == ASSIGNMENT_OP)
            {
                string key = line[0].first;
                
                lexicalAnalyser::tokenLineType expression = line;
                expression.erase(expression.begin(), expression.begin() + 2);

                vector<string> tempvector;
                tempvector.push_back(symbol.toPost(expression));
                lexAnalysis2.tokenGenerate(tempvector);
                
                if (!lexAnalysis2.getTokens().empty()) {
                    string data = to_string(symbol.evaluate(lexAnalysis2.getTokens()[0]));
                    
                    symbol.symbolTable[key] = data;
                }
            }
        }
        else if (line[nextBit].second == COMMENT)
        {
            //do nothing
        }
        else if (line[0].first == "print")
        {
            doPrint(line, 1, symbol);
        }

        else if (line[0].first == "while")
        {
            ifTrue = false;
            sudo = false;

            string condition = "";
            int k = 1;
            while (k < line.size() && line[k].first != ":")
            {
                condition += line[k].first;
                k++;
            }
            
            lexicalAnalyser::tokenType loopBlock = extractBlock(allCode, i);
            
            runWhile(condition, loopBlock);
        }
        else if (line[0].first == "for")
        {
            ifTrue = false;
            sudo = false;

            if (line.size() > 3 && line[2].first == "in" && line[3].first == "range")
            {
                string iteratorVar = line[1].first;
                
                vector<int> args;
                int k = 5;
                while(k < line.size() && line[k].second != RIGHT_PAREN)
                {
                    if (line[k].second == NUMERIC_LITERAL)
                    {
                        args.push_back(stoi(line[k].first));
                    }
                    else if (line[k].second == IDENTIFIER)
                    {
                        if (symbol.symbolTable.count(line[k].first)) {
                            args.push_back(stoi(symbol.symbolTable[line[k].first]));
                        } else {
                            args.push_back(0);
                        }
                    }
                    k++;
                }

                int start = 0, end = 0, step = 1;
                
                if (args.size() == 1) {
                    end = args[0];
                }
                else if (args.size() == 2) {
                    start = args[0];
                    end = args[1];
                }
                else if (args.size() == 3) {
                    start = args[0];
                    end = args[1];
                    step = args[2];
                }
                
                //get the indented block
                lexicalAnalyser::tokenType loopBlock = extractBlock(allCode, i);
                
                runFor(iteratorVar, start, end, step, loopBlock);
            }
            else
            {
                colors2.setColor(31);
                cout << "ERROR: Invalid for-loop syntax." << endl;
                colors2.resetColor();
            }
        }
        else if (line[0].first == "if")
        {
            sudo = false;
            string condition = "";
            int k = 1;
            while (k < line.size() && line[k].first != ":") {
                condition += line[k].first;
                k++;
            }
            ifTrue = conditionChecker(condition);
            if (ifTrue) sudo = true;
        }
        else if (line[0].first == "elif")
        {
            if (sudo) {
                ifTrue = false;
            } else {
                string condition = "";
                int k = 1;
                while (k < line.size() && line[k].first != ":") {
                    condition += line[k].first;
                    k++;
                }
                ifTrue = conditionChecker(condition);
                if (ifTrue) sudo = true;
            }
        }
        else if (line[0].first == "else")
        {
            if (sudo) {
                ifTrue = false;
            } else {
                ifTrue = true;
            }
        }
        else if (line[0].second == INDENT)
        {
            if (ifTrue)
            {
                lexicalAnalyser::tokenType tempCode;
                lexicalAnalyser::tokenLineType unindentedLine = line;
                unindentedLine.erase(unindentedLine.begin());
                tempCode.push_back(unindentedLine);
                
                //recursively call interpretLine on the unindented stuff
                int dummyI = 0;
                interpretLine(tempCode, dummyI);
            }
        }
    }
}

void Interpreter::runWhile(string conditional, lexicalAnalyser::tokenType& block)
{
    while (conditionChecker(conditional))
    {
        executeBlock(block);
    }
}

void Interpreter::runFor(string iteratorVar, int start, int end, int step, lexicalAnalyser::tokenType& block)
{
    if (step > 0)
    {
        for (int j = start; j < end; j += step)
        {
            symbol.symbolTable[iteratorVar] = to_string(j);
            executeBlock(block);
        }
    }
    else if (step < 0)
    {
        for (int j = start; j > end; j += step)
        {
            symbol.symbolTable[iteratorVar] = to_string(j);
            executeBlock(block);
        }
    }
}

bool Interpreter::conditionChecker(string conditional)
{
    int answer = symbol.processEquation(conditional);
    return (answer != 0);
}

void Interpreter::doPrint(lexicalAnalyser::tokenLineType line, int currentBit, Expevaluator& abacus)
{
    string outputString = "";

    if (line.size() > currentBit && line[currentBit].first == "(")
    {
        currentBit++;
        while (currentBit < line.size() && line[currentBit].second != RIGHT_PAREN)
        {
            if (line[currentBit].second == STRING_LITERAL)
            {
                outputString += line[currentBit].first.substr(1, line[currentBit].first.length() - 2);
            }
            else if (line[currentBit].second == COMMA)
            {
                //do nothing please
            }
            else
            {
                lexicalAnalyser::tokenLineType expr;
                while(currentBit < line.size() && line[currentBit].second != COMMA && line[currentBit].second != RIGHT_PAREN)
                {
                    expr.push_back(line[currentBit]);
                    currentBit++;
                }
                
                currentBit--;

                vector<string> tempVectorGuy;
                tempVectorGuy.push_back(abacus.toPost(expr));
                lexAnalysis2.tokenGenerate(tempVectorGuy);
                
                if (!lexAnalysis2.getTokens().empty()) {
                   outputString += to_string(abacus.evaluate(lexAnalysis2.getTokens()[0]));
                }
            }
            currentBit++;
        }
    }
    else
    {
        colors2.setColor(31);
        cout << "ERROR: Invalid print syntax" << endl;
        colors2.resetColor();
        return;
    }

    colors2.mintOn();
    cout << outputString << endl;
    colors2.resetColor();
}

void Interpreter::assignVariables(string key, string value)
{
    symbol.symbolTable[key] = value;
}
