//math stuff from project 5
#include <iostream>
#include "expevaluator.h"
#include <stack>
#include "interface.h"
#include <string>
using namespace std;

map<string, string> Expevaluator::symbolTable;

Interface colors;


int Expevaluator::processEquation(string equation)
{
    lexicalAnalyser lex;
    vector<string> tempvector;
    //send something to postfix conversion
    //and then evaluate it

    tempvector.push_back(equation);
    lex.tokenGenerate(tempvector);

    tempvector.clear();
    //lex.tokenShow(); //DEBUG CODE
    //cout << "POSTFIX: " << toPost(lex.tokenInfo[0]) << endl; //DEBUG CODE
    tempvector.push_back(toPost(lex.tokenInfo[0]));
    //cout << "DEBUG: pushed back postified thing: " << tempvector[0] << endl;
    
    lex.tokenGenerate(tempvector);
    //lex.tokenShow(); //DEBUG CODE
    return evaluate(lex.tokenInfo[0]);
}


int Expevaluator::precedence(string op)
{
    if (op == "or") {

        return 0;

    }

    if (op == "and") {

        return 1;

    }

    if (op == "not") {

        return 2;

    }

    if (op == "<" || op == "<=" || op == ">" || op == ">=" || op == "!=" || op == "==") {

        return 3;

    }

    if (op == "+" || op == "-") {

        return 4;

    }

    if (op == "*" || op == "/" || op == "%") {

        return 5;

    }

    return -1;
}


string Expevaluator::toPost(vector<pair<string, categoryType>> infix)
{

    //input system.
    string postfix = "";
    string temp;
    string killThis;

    stack<string> bingus;

    int leftCount = 0;
    int rightCount = 0;
    
    for (int o = 0; o < infix.size(); o++) {

        //cout << "DEBUG: Infix " << o << ": " << infix[o].first << endl;

    }
    
    for (int i = 0; i < infix.size(); i++)
    {
        if (infix[i].first == "int")
        {
            i++;

            //make sure the pattern continues for the int(input etc. thing
            if (infix[i].first == "(")
            {
                leftCount++;
                i++;

                //make sure the pattern continues
                if (infix[i].first == "input")
                {
                    i++;

                    if (infix[i].first == "(")
                    {
                        leftCount++;
                        i++;
                    }
                    else
                    {
                        //error color
                        colors.setColor(31);
                        cout << "ERROR: INPUT NEEDS LEFT PARENTHESES" << endl;
                        colors.resetColor();
                        break;
                    }
                    string input = collectInput(infix, i);

                    if (infix.size() > i + 1 && infix[i].first == ")" && infix[i + 1].first == ")")
                    {
                        i++;
                        postfix = postfix + input + " ";
                        rightCount += 2;
                    }
                    else
                    {
                        //error color
                        colors.setColor(31);
                        cout << "ERROR: MISSING RIGHT PARENTHESIS AT THE END" << endl;
                        colors.resetColor();
                    }
                }
                else
                {
                    //error color
                    colors.setColor(31);
                    cout << "ERROR: MISSING AN INPUT STATEMENT AFTER INT" << endl;
                    colors.resetColor();
                }


            }
            else
            {
                //error color
                colors.setColor(31);
                cout << "ERROR: MISSING LEFT PAREN" << endl;
                colors.resetColor();
            }


        }










        //above is the input stuff

        else if (infix[i].second == ARITH_OP
            || infix[i].second == LOGICAL_OP
            || infix[i].second == RELATIONAL_OP)
        {
            //cout << "DEBUG: operator is " << infix[i].first << endl;
            while (!bingus.empty() && bingus.top() != "(" && precedence(bingus.top()) >= precedence(infix[i].first))
            {
                //cout << "postfix "
                temp = bingus.top();
                bingus.pop();
                //cout << "DEBUG: postfix is currently, (before appending):" << postfix << endl; //DEBUG CODE
                postfix = postfix + " " + temp;
            }
            bingus.push(infix[i].first);
            //cout << "DEBUG: bingus.top() is " << bingus.top() << endl;
            //cout << "DEBUG: i is " << i << " size is " << infix.size() << endl;

        }
        else if (infix[i].second == NUMERIC_LITERAL || infix[i].second == IDENTIFIER)
        {
            //cout << "DEBUG: postfix is currently, (before appending):" << postfix << endl; //DEBUG CODE
            postfix = postfix + " " + infix[i].first;
            //cout << "DEBUG: postfix is currently, (AFTER appending):" << postfix << endl; //DEBUG CODE
        }
        else if (infix[i].second == LEFT_PAREN)
        {
            bingus.push(infix[i].first);
            leftCount++;
        }
        else if (infix[i].second == RIGHT_PAREN)
        {
            rightCount++;
            if (!bingus.empty())
            {
                while (bingus.top() != "(")
                {
                    //cout << "Top is " << bingus.top() << endl; //DEBUG CODE
                    temp = bingus.top();
                    bingus.pop();


                    //cout << "postfix is currently, (before appending):" << postfix << endl; //DEBUG CODE

                    postfix = postfix + " " + temp;
                    //cout << "postfix is currently, (AFTER appending):" << postfix << endl; //DEBUG CODE
                    if (bingus.empty())
                    {
                        //cout << "the current top is: " << bingus.top() << endl; //DEBUG CODE
                        break;
                    }
                }
                if (!bingus.empty())
                {
                    //cout << "the current top is: " << bingus.top() << endl; //DEBUG CODE
                    bingus.pop();
                }
            }
        }

    }

    while (!bingus.empty())
    {
        temp = bingus.top();
        //cout << "DEBUG: temp is:                                 " << temp << endl;
        bingus.pop();
        postfix = postfix + " " + temp;
    }
    if (leftCount != rightCount)
    {
        colors.setColor(33);
        cout << "----------------------------------------------------------------------------\n";
        cout << "You have a different amount of left parentheses as you do right parentheses." << endl << "Your answer... well... let's just say it MIGHT not be what you expect..." << endl;
        cout << "----------------------------------------------------------------------------\n";
        colors.resetColor();
    }
    return postfix;
}


int Expevaluator::evaluate(lexicalAnalyser::tokenLineType postfix)
{
    stack<int> ev;
    for (int i = 0; i < postfix.size(); i++)
    {
        //cout << "DEBUG: the current postfix: " << postfix[i].first << endl;
        if (postfix[i].second == categoryType::NUMERIC_LITERAL)
        {
            ev.push(stoi(postfix[i].first));
        }
        else if (postfix[i].second == categoryType::ARITH_OP
            || (postfix[i].second == categoryType::LOGICAL_OP && postfix[i].first != "not")
            || postfix[i].second == categoryType::RELATIONAL_OP)
        {
            int operand2 = ev.top();
            ev.pop();
            int operand1 = ev.top();
            //line below this is where we die
            ev.pop();
            ev.push(calculate(postfix[i].first, operand1, operand2));
        }
        else if (postfix[i].second == categoryType::IDENTIFIER)
        {
            //cout << "DEBUG: is identifier? currently at: " << postfix[i].first << endl;
            auto it = symbolTable.find(postfix[i].first);
            if (it != symbolTable.end())
            {
                //cout << "DEBUG: found it, value is: " << it->second << endl;
                ev.push(stoi(it->second));
                //cout << "DEBUG: push successful." << endl;
            }
            else
            {
                ev.push(0);
                //error color
                colors.setColor(31);
                cout << "ERROR: identifier not in symbolTable, pushing 0 instead cuh" << endl;
                colors.resetColor();
            }
        }
        else if (postfix[i].second == categoryType::ASSIGNMENT_OP)
        {
            //Do nothing for the millionth time
        }
        else if (postfix[i].second == categoryType::RIGHT_PAREN)
        {
            //Do nothing for the millionth time
        }
        else if (postfix[i].first == "not")
        {
            int op = ev.top();
            ev.pop();
            ev.push(!op);
        }
        else if (postfix[i].second == categoryType::COMMENT)
        {
            //do nothing
        }
        else
        {
            //cout << "ERROR: unexpected token cuh: " << postfix[i].second << endl;

        }
    }
    //cout << "DEBUG: trying to return: " << ev.top() << endl;
    return ev.top();
}

int Expevaluator::calculate(string op, int op1, int op2)
{
    if (op == "+")
        return (op1 + op2);
    if (op == "-")
        return (op1 - op2);
    if (op == "/")
        return (op1 / op2);
    if (op == "*")
        return (op1 * op2);
    if (op == "%")
        return (op1 % op2);
    if (op == "<")
        return (op1 < op2);
    if (op == ">")
        return (op1 > op2);
    if (op == "<=")
        return (op1 <= op2);
    if (op == ">=")
        return (op1 >= op2);
    if (op == "!=")
        return (op1 != op2);
    if (op == "==")
        return (op1 == op2);
    if (op == "and")
        return (op1 && op2);
    if (op == "or")
        return (op1 || op2);
    return 0;
}

void Expevaluator::displayTable()
{
    if (!symbolTable.empty())
    {
        colors.mint2On();
        cout << "******* Symbol Table Variables and Values *******" << endl;
        colors.resetColor();

        map<string, string>::iterator it;

        for (it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            colors.mintOn();
            cout << it->first << " = " << it->second << endl;
            colors.resetColor();
        }
    }
    else
    {
        //error color
        colors.setColor(31);
        cout << "ERROR: Symbol Table is empty." << endl;
        colors.resetColor();
    }
}


string Expevaluator::collectInput(lexicalAnalyser::tokenLineType line, int& i)
{
    string input = "";
    while (line[i].second != RIGHT_PAREN)
    {
        if (line[i].second == STRING_LITERAL)
        {
            colors.mintOn();
            cout << line[i].first.substr(1, line[i].first.length() - 2);
            colors.resetColor();
        }
        else if (line[i].second == COMMA)
        {
            //do nothing!
        }
        else if (line[i].second == IDENTIFIER)
        {
            colors.mintOn();
            cout << symbolTable.find(line[i].first)->second;
            colors.resetColor();
        }
        else
        {
            //error color
            colors.setColor(31);
            cout << "ERROR: INVALID INPUT STATEMENT" << endl;
            colors.resetColor();
            i++;
            return input;
        }
        i++;
    }
    getline(cin, input);
    return input;
}
