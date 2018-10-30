#ifndef SEMANT_H
#define SEMANT_H

#include "def.h"
#include "scaner.h"

#define EMPTY -1

struct Node{
    TLexem id;
    DataType type;
    char *data;
};

class Tree
{
    Node *n;
    Tree *Up, *Left, *Right;

public:
    static Tree *Curr;
    static TScaner *sc;

    Tree(Tree *l, Tree *r, Tree *u, Node *data);
    Tree();
    ~Tree();

    void SetLeft(Node *data);
    void SetRight(Node *data);
    Tree* FindUp(Tree *from, TLexem id);
    Tree* FindUpOneLevel(Tree* from, TLexem id);
    Tree* FindUp(TLexem id);

    //semantic functions

    //установить текущий узел
    void SetCurr(Tree *a);

    //получить текущий узел
    Tree* GetCurr();

    //занесение ид
    Tree* Include(TLexem id, DataType t);

    Tree* SemGetVar(TLexem a);

    void SemSetType(Tree *addr, DataType t);

    Tree* SemGetFunc(TLexem a);

    int DuplicateControl(Tree *addr, TLexem a);

    DataType SemType(TLexem type);

    Tree* makeBlock();

};

#endif // SEMANT_H
