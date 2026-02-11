#pragma once
#ifndef LEXANALYSIS_H
#define LEXANALYSIS_H
#include <vector>
#include <string>
using namespace std;

enum categoryType
{
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN
};



class lexicalAnalyser
{
private:
    typedef vector<vector<pair<string, categoryType>>> tokenType;
    typedef vector<pair<string, categoryType>> tokenLineType;
    typedef pair<string, categoryType> pairType;
    tokenLineType lineTokens;
    string categoriesSorted[15]
    {
        "KEYWORD",
        "IDENTIFIER",
        "STRING_LITERAL",
        "NUMERIC_LITERAL",
        "ASSIGNMENT_OP",
        "ARITH_OP",
        "LOGICAL_OP",
        "RELATIONAL_OP",
        "LEFT_PAREN",
        "RIGHT_PAREN",
        "COLON",
        "COMMA",
        "COMMENT",
        "INDENT",
        "UNKNOWN"
    };
public:
    void tokenGenerate(vector<string> programCode);
    void tokenShow();
    void tokenClear();
    tokenType tokenInfo;
    const tokenType getTokens() {
        return tokenInfo;
    }
    friend class Interpreter;
    friend class Expevaluator;
};

#endif
