#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "tokens.h"

#include <string>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

class Interpreter
{
public:

    Interpreter();
    ~Interpreter();

    Token_e inferTokenType(char ch) const;
    void start(char** argv, int argc);

private:

    void parse(string&);
    void execute();
    void instruction(Token_e opcode);

    ifstream m_inputfile;

    deque<Token_e> m_runtime;
    deque<Token_e>::const_iterator m_dequeIterator;

    string m_cmdLine;

    vector<deque<Token_e>::const_iterator> m_iterators; // For the stack of loops

    signed short m_cx;
    unsigned char* m_cell;
};

#endif
