#ifndef SYNTAX_H
#define SYNTAX_H
#include <scaner.h>
#include <memuk.h>

class Syntax
{
private:
    TScaner *sc;
    void descript(); //описания
    void mainfunc(); //функция main
    void data(); //данные
    void operAndDesc(); //операторы и описания
    void operat(); //оператор
    void compOperator(); //составной оператор
    void expression(); //выражение
    void mult(); //множитель
    void prefix(); //префикс
    void postfix(); //постфикс
    void elem(); //элементарное выражение
    void switchOperator(); //switch
    void cases();
    void operators(); //операторы
public:
    Syntax(TScaner *sc);
    void program();
};

#endif // SYNTAX_H
