#ifndef DEF_H
#define DEF_H

#define MAXLEN 30
#define TEXTLEN 10000
#define NUMLEN 10
#define KEYWORDS 10

typedef char TLexem[MAXLEN];

enum en{ID = 1, CONSTINT10, CONSTINT8,
        CONSTINT16, CONSTCHAR,
        PLUS = 11, MINUS, DIV, MULT, PERCENT,
        INCREMENT, DECREMENT,
        SEMICOLON = 20, OPENBRACKET,
        CLOSEBRACKET, OPENBRACE, CLOSEBRACE, COLON, COMMA,
        ASSIGN, SWITCH, CASE, DEFAULT, BREAK,
        CHAR = 50, INT, DOUBLE,
       CLASS = 60, VOID, MAIN, MAINFUNC, ERROR = 100, END = 200};

#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

#endif // DEF_H
