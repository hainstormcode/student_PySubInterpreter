#include <iostream>
#include <string>
#include <utility>
#include <iterator>
#include <fstream>
#include "interface.h"
#include "lexAnalysis.h"
using namespace std;

Interface gui;


void lexicalAnalyser::tokenGenerate(vector<string> programCode)
{
    tokenClear();
    gui.setColor(94);
    int lineNum = 0;
    //showing lines and tokens system
    
    if (!programCode.empty())
    {
        for (auto i : programCode) {
            lineTokens.clear();
            lineNum++;
            //cout << "I is " << i << endl;
            for (int k = 0; k < i.size(); k++)
            {
            //cout << "DEBUG: being postfixized:" << i.at(k) << endl;
                int start = 0;
                char tidbit = i.at(k);
                if (tidbit == '#')
                {
                    //cout << "this is the comment" << endl;
                    string token = i.substr(k, i.size());
                    lineTokens.push_back(make_pair(token, categoryType::COMMENT));
                    k = i.size();
                }
                else if (isblank(tidbit))
                {
                    //cout << "this is the space" << endl;
                    if (k == 0)
                    {
                        //cout << "this is the k == 0" << endl;
                        //store whitespace at start of line as INDENT
                        start = k;
                        while (isblank(i.at(k)) && k < i.size()) {
                            k++;
                            tidbit = i.at(k);
                            if (k == i.size()) {
                                break;
                            }
                        }
                        k--;
                        tidbit = i.at(k);
                        int length = k - start;
                        string token = i.substr(start, length);
                        lineTokens.push_back(make_pair(token, categoryType::INDENT));
                    }
                    else
                    {
                        //ignore it and read next character
                    }
                }
                else if (tidbit == '<' || tidbit == '>' || tidbit == '!')
                {
                    //cout << "this is the < > !" << endl;
                    string token = "";
                    token += tidbit;
                    
                    
                    if (k + 1 < i.size() && i.at(k + 1) == '=')
                    {
                        //cout << "this is the tidbit =" << endl;
                        k++;
                        token += '=';
                    }
                    
                    
                    lineTokens.push_back(make_pair(token, categoryType::RELATIONAL_OP));
                }
                else if (tidbit == '+' || tidbit == '-' || tidbit == '*' || tidbit == '/' || tidbit == '%')
                {
                    //cout << "this is the + - * / %" << endl;
                    start = k;
                    string token = i.substr(start, 1);
                    lineTokens.push_back(make_pair(token, categoryType::ARITH_OP));
                }
                else if (isalpha(tidbit))
                {
                    //cout << "this is the alpha" << endl;
                    start = k;
                    while(isalpha(i.at(k)) && k < i.size()) {
                        k++;
                        if (k == i.size()) {
                            break;
                        }
                        //cout << "the alpha is: " << i.substr(start, k - start) << endl;
                    }
                    int length = k - start;
                    //cout << "hellor this is substring: " << i.substr(start, length) << endl;
                    string token = i.substr(start, length);
                    
                    if (token == "and" || token == "or" || token == "not" || token == "in")
                    {
                        //cout << "this is the and or not" << endl;
                        lineTokens.push_back(make_pair(token, categoryType::LOGICAL_OP));
                    }
                    else if (token == "print" || token == "if" || token == "elif" || token == "else" || token == "while" || token == "int" || token == "input" || token == "for" || token == "range")
                    {
                        //cout << "this is the print if elif else while int input" << endl;
                        lineTokens.push_back(make_pair(token, categoryType::KEYWORD));
                    }
                    else
                    {
                        lineTokens.push_back(make_pair(token, categoryType::IDENTIFIER));
                    }
                    k--;
                    tidbit = i.at(k);
                }
                else if (tidbit == '=')
                {
                    start = k;

                    char equalChecker = i.at(k + 1);
                    //cout << "THIS IS WHAT EQUAL CHECKER FOUND: " << equalChecker << endl;
                    //cout << "this is the = initial" << endl;
                    if (equalChecker == '=')
                    {
                        //cout << "this is the tidbit =" << endl;
                        k++;
                        int length = k - start;
                        string token = "==";
                        lineTokens.push_back(make_pair(token, categoryType::RELATIONAL_OP));
                    }
                    else
                    {
                        string token1(1, '=');
                        lineTokens.push_back(make_pair(token1, categoryType::ASSIGNMENT_OP));
                    }
                }
                else if (tidbit == '(')
                {
                    //cout << "this is the (" << endl;
                    string token(1, tidbit);
                    lineTokens.push_back(make_pair(token, categoryType::LEFT_PAREN));
                }
                else if (tidbit == ')')
                {
                    //cout << "this is the )" << endl;
                    string token(1, tidbit);
                    lineTokens.push_back(make_pair(token, categoryType::RIGHT_PAREN));
                }
                else if (tidbit == ':')
                {
                    //cout << "this is the :" << endl;
                    string token(1, tidbit);
                    lineTokens.push_back(make_pair(token, categoryType::COLON));
                }
                else if (tidbit == ',')
                {
                    //cout << "this is the ," << endl;
                    string token(1, tidbit);
                    lineTokens.push_back(make_pair(token, categoryType::COMMA));
                }
                else if (isdigit(tidbit))
                {
                    //cout << "this is the digit" << endl;
                    start = k;
                    while (k < i.size() && isdigit(i.at(k)))
                    {
                        k++;
                        if (k == i.size()) {
                            break;
                        }
                        //cout << "this yo digey cuh " << i.substr(start, k - start) << endl;
                    }
                    //cout << "before int length digey cuh" << endl;
                    int length = k - start;
                    //cout << "before substr digey cuh" << endl;
                    string token = i.substr(start, length);
                    //cout << "DEBUG: sending token:          " << token << endl;
                    //cout << "before pushback digey cuh" << endl;
                    //cout << "token is: " << token << endl;
                    lineTokens.push_back(make_pair(token, categoryType::NUMERIC_LITERAL));
                    //cout << "after pushback digey cuh" << endl;
                    k--;
                    

                    
                }
                //single quote interceptor
                else if (tidbit == '\'')
                {
                    start = k;
                    //cout << "this is the single quote" << endl;
                    k++;
                    tidbit = i.at(k);
                    while (tidbit != '\'' && k < i.size()) {
                        k++;
                        if (k == i.size()) {
                            break;
                        }
                        tidbit = i.at(k);
                    }
                    int length = 1 + k - start;
                    string token = i.substr(start, length);
                    lineTokens.push_back(make_pair(token, categoryType::STRING_LITERAL));
                    tidbit = i.at(k);
                }
                //double quote interceptor
                else if (tidbit == '\"')
                {
                    start = k;
                    //cout << "this is the double quote" << endl;
                    k++;
                    tidbit = i.at(k);
                    while (tidbit != '\"' && k < i.size()) {
                        k++;
                        if (k == i.size()) {
                            break;
                        }
                        tidbit = i.at(k);
                    }
                    int length = 1 + k - start;
                    string token = i.substr(start, length);
                    lineTokens.push_back(make_pair(token, categoryType::STRING_LITERAL));
                    tidbit = i.at(k);
                }
                else if (isspace(tidbit))
                {
                    
                }
                else
                {
                    string token = i;
                    lineTokens.push_back(make_pair(token, categoryType::UNKNOWN));
                }
            }
            tokenInfo.push_back(lineTokens);

            
        }
    }



    else
    {
        cout << "No file read into memory." << endl;
        gui.resetColor();
    }
    gui.resetColor();
}




void lexicalAnalyser::tokenShow()
{
    gui.setColor(94);
    int lineNum = 0;
    int tokNum = 0;
    if (!tokenInfo.empty())
    {
        gui.setColor(96);
        cout << "***** TOKEN INFORMATION *****" << endl;
        gui.setColor(94);
        //cout << "Line #" << lineNum << ":" << endl;
        //cout << "Token[" << tokNum << "]:   ";
    

        for (int i = 0; i < tokenInfo.size(); i++)
        {
            gui.setColor(94);
            cout << endl << "----------------------------------------------------------" << endl;
            cout << "Line " << i << ":" << endl;
            for (int j = 0; j < tokenInfo[i].size(); j++) {
                gui.setColor(94);
                cout << "Token[" << j << "] ";
                gui.setColor(96);
                cout << tokenInfo[i][j].first << " - " << categoriesSorted[tokenInfo[i][j].second] << endl;
            }

        }
    }
    gui.resetColor();
}

void lexicalAnalyser::tokenClear()
{
    lineTokens.clear();
    tokenInfo.clear();
}
