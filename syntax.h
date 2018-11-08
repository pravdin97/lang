#ifndef SYNTAX_H
#define SYNTAX_H
#include <scaner.h>
#include <memuk.h>
#include <semant.h>

class Syntax
{
private:
    TScaner *sc;

    Tree *root;
    void descript(); //описания
    void mainfunc(); //функция main
    void data(); //данные
    void operAndDesc(); //операторы и описания
    void operat(); //оператор
    void compOperator(); //составной оператор
    DataType expression(); //выражение
    DataType mult(); //множитель
    void prefix(); //префикс
    void postfix(); //постфикс
    DataType elem(); //элементарное выражение
    void switchOperator(); //switch
    void cases();
    void operators(); //операторы
public:
    Syntax(TScaner *sc);
    void program();
};

#endif // SYNTAX_H
