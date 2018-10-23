#ifndef SCANER_H
#define SCANER_H
#include "def.h"
#include "memuk.h"

class TScaner
{
private:
    char *text; //текст модуля
    int uk; //текущее положение указателя
    int line, pos; //для ошибки
    MemUK mem;
public:
    TScaner();
    void setUk(MemUK memUk);
    MemUK getUk();
    int scaner(TLexem lex);
    void openFile(char* path);
    int compare(TLexem lex);
    void printError(char *err, TLexem l);
};

#endif // SCANER_H
