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
    if (t != CLASS) sc->printError("ќжидалось class", l);

    t = sc->scaner(l);
    if (t != MAIN) sc->printError("ќжидалось Main", l);

    t = sc->scaner(l);
    if (t != OPENBRACE) sc->printError("ќжидалось {", l);

    descript();

    t = sc->scaner(l);
    if (t != CLOSEBRACE) sc->printError("ќжидалось }", l);
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
    if (t != VOID) sc->printError("ќжидалось void", l);

    t = sc->scaner(l);
    if (t != MAINFUNC) sc->printError("ќжидалось main", l);

    Tree* cur = root->Include(l, semFunc);

    t = sc->scaner(l);
    if (t != OPENBRACKET) sc->printError("ќжидалось (", l);

    t = sc->scaner(l);
    if (t != CLOSEBRACKET) sc->printError("ќжидалось )", l);

    compOperator();

    root->Curr = cur;
}

void Syntax::data(){
    TLexem l;
    int t, uk1;

    t = sc->scaner(l);
    if (t != INT && t != DOUBLE) sc->printError("ќжидалось int или double", l);

    DataType type = root->SemType(l);
    DataType typ2;

    do{
        t = sc->scaner(l);
        if (t != ID) sc->printError("ќжидалс€ идентификатор", l);

        root->Include(l, type);

        //----------------------------
        t = sc->scaner(l);

        if (t == ASSIGN)
        {
            typ2 = expression();
            root->typeAccord(type, typ2);
            t = sc->scaner(l);
        }

        //----------------------------

        //t = sc->scaner(l);
    }while(t == COMMA);

    if (t != SEMICOLON) sc->printError("ќжидалось ;", l);
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
    if (t != OPENBRACE) sc->printError("ќжидалось {", l);

    Tree* curr = root->makeBlock();

    operAndDesc();

    root->Curr = curr;

    t = sc->scaner(l);
    if (t != CLOSEBRACE) sc->printError("ќжидалось }", l);
}

void Syntax::operat(){
    TLexem l;
    int t, uk;
    MemUK uk1;
    DataType typ1, typ2;

    uk1 = sc->getUk();
    t = sc->scaner(l);
    if (t == ID)
    {
        Tree* tr = root->SemGetVar(l);
        typ1 = tr->getType();

        t = sc->scaner(l);
        if (t != ASSIGN) sc->printError("ќжидалось =", l);

        typ2 = expression();

        root->typeAccord(typ1, typ2);

        t = sc->scaner(l);
        if (t != SEMICOLON) sc->printError("ќжидалось ;", l);
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
        sc->printError("ќжидалось ;", l);
}

DataType Syntax::expression(){
    TLexem l;
    int t, uk;
    MemUK uk1;
    DataType typ1, typ2;

    typ1 = mult();

    uk1 = sc->getUk();
    t = sc->scaner(l);
    //sc->setUk(uk1);

    while(t == PLUS || t == MINUS)
    {
        typ2 = mult();
        typ1 = root->castMult(typ1, typ2);

        uk1 = sc->getUk();
        t = sc->scaner(l);
    }

    sc->setUk(uk1);

    return typ1;
}

DataType Syntax::mult(){
    TLexem l;
    int t, uk;
    MemUK uk1;
    DataType typ1, typ2;

    typ1 = elem();

    uk1 = sc->getUk();
    t = sc->scaner(l);
    //sc->setUk(uk1);

    while(t == MULT || t == DIV || t == PERCENT)
    {
        typ2 = elem();
        typ1 = root->castMult(typ1, typ2);

        uk1 = sc->getUk();
        t = sc->scaner(l);
    }

    sc->setUk(uk1);

    return typ1;
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

//пп должны возвращать семантический тип?

DataType Syntax::elem(){
    TLexem l;
    int t;
    MemUK uk1;
    DataType type;

    t = sc->scaner(l);

    if (t == OPENBRACKET)
    {
        type = expression();

        t = sc->scaner(l);
        if (t != CLOSEBRACKET) sc->printError("ќжидалось )", l);
    }
    else if (t == INCREMENT || t == DECREMENT || t == PLUS || t == MINUS)
    {
        t = sc->scaner(l);
        if (t != ID) sc->printError("ќжидалс€ идентификатор", l);

        Tree* node = root->SemGetVar(l);
        type = node->getType();
    }
    else if (t == ID)
    {
        Tree* node = root->SemGetVar(l);
        type = node->getType();

        uk1 = sc->getUk();
        t = sc->scaner(l);

        if (t == INCREMENT || t == DECREMENT)
            ;
        else sc->setUk(uk1);
    }
    else if (t != ID && t != CONSTCHAR && t != CONSTINT8 && t != CONSTINT10
             && t != CONSTINT16)
        sc->printError("ќжидалось константа или идентификатор", l);

    return type;
}

void Syntax::switchOperator(){
    TLexem l;
    int t;

    t = sc->scaner(l);
    if (t != SWITCH) sc->printError("ќжидалось switch", l);

    t = sc->scaner(l);
    if (t != OPENBRACKET) sc->printError("ќжидалось (", l);

    int type = expression();
    if (type != semInt)
        sc->errMsg("ќжидалось int", "");

    t = sc->scaner(l);
    if (t != CLOSEBRACKET) sc->printError("ќжидалось )", l);

    t = sc->scaner(l);
    if (t != OPENBRACE) sc->printError("ќжидалось {", l);

    cases();

    t = sc->scaner(l);
    if (t == DEFAULT)
    {
        t = sc->scaner(l);
        if (t != COLON) sc->printError("ќжидалось :", l);

        operators();

        t = sc->scaner(l);
    }

    if (t != CLOSEBRACE) sc->printError("ќжидалось }", l);
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
                sc->printError("ќжидалась константа", l);

        t = sc->scaner(l);
        if (t != COLON) sc->printError("ќжидалось :", l);

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
            if (t != SEMICOLON) sc->printError("ќжидалось ;", l);
        }
        else
            sc->printError("ќжидалс€ оператор", l);

        uk1 = sc->getUk();
        t = sc->scaner(l);
        sc->setUk(uk1);
    }while(t == BREAK || t == ID || t == OPENBRACE || t == SWITCH || t == SEMICOLON);
}






