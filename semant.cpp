#include "semant.h"

Tree* Tree::Curr = (Tree*)NULL;
TScaner* Tree::sc = (TScaner*)NULL;

TLexem TYPES[NUMSEMTYPES] = {"int", "double"};
DataType SEMTYPES[NUMSEMTYPES] = {semInt, semDouble};

Tree::Tree(Tree *l, Tree *r, Tree *u, Node *data){
    n = new Node();
    Up = u;
    Left = l;
    Right = r;
    memcpy(n, data, sizeof(Node));
}

Tree::Tree()
{
    n = new Node();
    Up = NULL; Right = NULL; Left = NULL;
    memcpy(n, &("------"), sizeof(Node));
}

void Tree::SetLeft(Node *data)
{
    Tree *a = new Tree(NULL, NULL, this, data);
    Left = a;
}

void Tree::SetRight(Node *data)
{
    Tree *a = new Tree(NULL, NULL, this, data);
    Right = a;
}

Tree* Tree::FindUp(TLexem id)
{
    return FindUp(this, id);
}

Tree* Tree::FindUp(Tree *from, TLexem id)
{
    Tree *i=from;
    while( (i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0) )
        i = i->Up;
    return i;
}

Tree* Tree::FindUpOneLevel(Tree *from, TLexem id)
{
    Tree *i = from;
    while( (i != NULL) && (i->Up != NULL) && (i->Up->Right != i))
    {
        if (memcmp(id, i->n->id, max(strlen(i->n->id) - 1, strlen(id))) == 0)
            return i;
        i = i->Up;
    }
    return NULL;
}

void Tree::SetCurr(Tree *a) { Curr = a; }

Tree* Tree::GetCurr() { return Curr; }

int Tree::DuplicateControl(Tree *addr, TLexem a)
{
    if (FindUpOneLevel(addr, a) == NULL)
        return 0;
    return 1;
}

Tree* Tree::Include(TLexem id, DataType t)
{
    if (DuplicateControl(Curr, id))
         sc->errMsg("Повторное описание идентификатора", id);

    Tree *v;
    Node b;

    if (t == semFunc)
    {
        memcpy(b.id, id, strlen(id) + 1);
        b.type = t;
        b.data = NULL;
        Curr->SetLeft(&b);
        Curr = Curr->Left;
        v = Curr;

        memcpy(&b.id, &"", 2);
        b.type = semUndefine;
        b.data = NULL;

        Curr->SetRight(&b);
        Curr = Curr->Right;
        return v;
    }
    else
    {
        memcpy(b.id, id, strlen(id) + 1);
        b.type = t;
        b.data = NULL;
        Curr->SetLeft(&b);
        Curr = Curr->Left;
        return Curr;
    }
}

Tree* Tree::makeBlock()
{
    Tree *v;
    Node b;

    memcpy(&b.id, &"", 2);
    b.type = semUndefine;
    b.data = NULL;

    Curr->SetLeft(&b);
    Curr = Curr->Left;
    v = Curr;

    memcpy(&b.id, &"", 2);
    b.type = semUndefine;
    b.data = NULL;

    Curr->SetRight(&b);
    Curr = Curr->Right;
    return v;
}

Tree* Tree::SemGetVar(TLexem a)
{
    Tree *v = FindUp(Curr, a);
    if (v == NULL)
         sc->errMsg("Отсутствует описание идентификатора", a);
    return v;
}

void Tree::SemSetType(Tree *addr, DataType t)
{
    addr->n->type = t;
}

Tree* Tree::SemGetFunc(TLexem a)
{
    Tree *v = FindUp(Curr, a);
    if (v == NULL)
        sc->errMsg("Отсутствует описание функции", a);
    if (v->n->type != semFunc)
        sc->errMsg("Не является функцией",a);
    return v;
}

DataType Tree::SemType(TLexem type)
{
    for (int i = 0 ; i < NUMSEMTYPES; i++)
        if (strcmp(type, TYPES[i]) == 0)
            return SEMTYPES[i];
    return semUndefine;
}
