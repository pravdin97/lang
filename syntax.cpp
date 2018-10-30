#include "syntax.h"

Syntax::Syntax(TScaner *sc)
{
    this->sc = sc;
    root = new Tree();
    root->Curr = root;
    root->sc = sc;
}

void Syntax::program(){
    TLexem l;
    int t;
    t = sc->scaner(l);
    if (t != CLASS) sc->printError("��������� class", l);

    t = sc->scaner(l);
    if (t != MAIN) sc->printError("��������� Main", l);

    t = sc->scaner(l);
    if (t != OPENBRACE) sc->printError("��������� {", l);

    descript();

    t = sc->scaner(l);
    if (t != CLOSEBRACE) sc->printError("��������� }", l);
}

void Syntax::descript(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    uk1 = sc->getUk();
    t = sc->scaner(l);
    sc->setUk(uk1);
    while(t == VOID || t == INT || t == DOUBLE){
        if (t == VOID)
            mainfunc();
        else data();
        uk1 = sc->getUk();
        t = sc->scaner(l);
        sc->setUk(uk1);
    }
}

void Syntax::mainfunc(){
    TLexem l;
    int t;

    t = sc->scaner(l);
    if (t != VOID) sc->printError("��������� void", l);

    t = sc->scaner(l);
    if (t != MAINFUNC) sc->printError("��������� main", l);

    Tree* cur = root->Include(l, semFunc);

    t = sc->scaner(l);
    if (t != OPENBRACKET) sc->printError("��������� (", l);

    t = sc->scaner(l);
    if (t != CLOSEBRACKET) sc->printError("��������� )", l);

    compOperator();

    root->Curr = cur;
}

void Syntax::data(){
    TLexem l;
    int t, uk1;

    t = sc->scaner(l);
    if (t != INT && t != DOUBLE) sc->printError("��������� int ��� double", l);

    DataType type = root->SemType(l);

    do{
        t = sc->scaner(l);
        if (t != ID) sc->printError("�������� �������������", l);

        root->Include(l, type);

        //----------------------------
        t = sc->scaner(l);

        if (t == ASSIGN)
        {
            expression();
            t = sc->scaner(l);
        }

        //----------------------------

        //t = sc->scaner(l);
    }while(t == COMMA);

    if (t != SEMICOLON) sc->printError("��������� ;", l);
}

void Syntax::operAndDesc(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    uk1 = sc->getUk();
    t = sc->scaner(l);
    sc->setUk(uk1);
    while(t == INT || t == DOUBLE || t == ID || t == OPENBRACE || t == SWITCH || t == SEMICOLON ){
        if (t == INT || t == DOUBLE)
            data();
        else operat();
        uk1 = sc->getUk();
        t = sc->scaner(l);
        sc->setUk(uk1);
    }
}

void Syntax::compOperator(){
    TLexem l;
    int t;

    t = sc->scaner(l);
    if (t != OPENBRACE) sc->printError("��������� {", l);

    Tree* curr = root->makeBlock();

    operAndDesc();

    root->Curr = curr;

    t = sc->scaner(l);
    if (t != CLOSEBRACE) sc->printError("��������� }", l);
}

void Syntax::operat(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    uk1 = sc->getUk();
    t = sc->scaner(l);
    if (t == ID)
    {
        t = sc->scaner(l);
        if (t != ASSIGN) sc->printError("��������� =", l);

        expression();

        t = sc->scaner(l);
        if (t != SEMICOLON) sc->printError("��������� ;", l);
    }
    else if (t == OPENBRACE)
    {
        sc->setUk(uk1);
        compOperator();
    }
    else if (t == SWITCH)
    {
        sc->setUk(uk1);
        switchOperator();
    }
    else if (t != SEMICOLON)
        sc->printError("��������� ;", l);
}

void Syntax::expression(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    mult();

    uk1 = sc->getUk();
    t = sc->scaner(l);
    //sc->setUk(uk1);

    while(t == PLUS || t == MINUS)
    {
        mult();

        uk1 = sc->getUk();
        t = sc->scaner(l);
    }

    sc->setUk(uk1);
}

void Syntax::mult(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    elem();

    uk1 = sc->getUk();
    t = sc->scaner(l);
    //sc->setUk(uk1);

    while(t == MULT || t == DIV || t == PERCENT)
    {
        elem();

        uk1 = sc->getUk();
        t = sc->scaner(l);
    }

    sc->setUk(uk1);
}

//???
void Syntax::prefix(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    uk1 = sc->getUk();
    t = sc->scaner(l);

    if (t == INCREMENT || t == DECREMENT || t == PLUS || t == MINUS)
        ;
    else sc->setUk(uk1);

    postfix();
}

void Syntax::postfix(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    elem();

    uk1 = sc->getUk();
    t = sc->scaner(l);

    if (t == INCREMENT || t == DECREMENT)
        ;
    else sc->setUk(uk1);
}

//�� ������ ���������� ������������� ���?

void Syntax::elem(){
    TLexem l;
    int t;
    MemUK uk1;

    t = sc->scaner(l);

    if (t == OPENBRACKET)
    {
        expression();

        t = sc->scaner(l);
        if (t != CLOSEBRACKET) sc->printError("��������� )", l);
    }
    else if (t == INCREMENT || t == DECREMENT || t == PLUS || t == MINUS)
    {
        t = sc->scaner(l);
        if (t != ID) sc->printError("�������� �������������", l);

        root->SemGetVar(l);
    }
    else if (t == ID)
    {
        root->SemGetVar(l);

        uk1 = sc->getUk();
        t = sc->scaner(l);

        if (t == INCREMENT || t == DECREMENT)
            ;
        else sc->setUk(uk1);
    }
    else if (t != ID && t != CONSTCHAR && t != CONSTINT8 && t != CONSTINT10
             && t != CONSTINT16)
        sc->printError("��������� ��������� ��� �������������", l);

}

void Syntax::switchOperator(){
    TLexem l;
    int t;

    t = sc->scaner(l);
    if (t != SWITCH) sc->printError("��������� switch", l);

    t = sc->scaner(l);
    if (t != OPENBRACKET) sc->printError("��������� (", l);

    expression();

    t = sc->scaner(l);
    if (t != CLOSEBRACKET) sc->printError("��������� )", l);

    t = sc->scaner(l);
    if (t != OPENBRACE) sc->printError("��������� {", l);

    cases();

    t = sc->scaner(l);
    if (t == DEFAULT)
    {
        t = sc->scaner(l);
        if (t != COLON) sc->printError("��������� :", l);

        operators();

        t = sc->scaner(l);
    }

    if (t != CLOSEBRACE) sc->printError("��������� }", l);
}

void Syntax::cases(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    uk1 = sc->getUk();
    t = sc->scaner(l);

    while(t == CASE)
    {
        t = sc->scaner(l);

        if ( t != CONSTCHAR && t != CONSTINT8 && t != CONSTINT10 && t != CONSTINT16)
                sc->printError("��������� ���������", l);

        t = sc->scaner(l);
        if (t != COLON) sc->printError("��������� :", l);

        operators();

        uk1 = sc->getUk();
        t = sc->scaner(l);
    }

    sc->setUk(uk1);
}

void Syntax::operators(){
    TLexem l;
    int t, uk;
    MemUK uk1;

    do{
        uk1 = sc->getUk();
        t = sc->scaner(l);
        if (t == ID || t == OPENBRACE || t == SWITCH || t == SEMICOLON)
        {
            sc->setUk(uk1);
            operat();
        }
        else if (t == BREAK)
        {
            t = sc->scaner(l);
            if (t != SEMICOLON) sc->printError("��������� ;", l);
        }
        else
            sc->printError("�������� ��������", l);

        uk1 = sc->getUk();
        t = sc->scaner(l);
        sc->setUk(uk1);
    }while(t == BREAK || t == ID || t == OPENBRACE || t == SWITCH || t == SEMICOLON);
}






