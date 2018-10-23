#include <iostream>
#include "def.h"
#include "scaner.h"
#include "syntax.h"
using namespace std;

int main()
{
    //LC_ALL=ru_RU.UTF-8;
    setlocale(0,"");
    TScaner scaner;

    scaner.openFile("F:/Projects/qt/lang.txt");

    Syntax syntax(&scaner);
    syntax.program();

    TLexem l;
    int t = scaner.scaner(l);
    if (t != END)
        scaner.printError("Обнаружент лишний текст после конца программы", l);
    else
        cout << "OK" << endl;

    return 0;
}
