#include "scaner.h"
#include "def.h"

TScaner::TScaner(){
    uk = 0;
    text = new char[TEXTLEN];
    line = 1; pos = 1;
}

TLexem KeyWord[KEYWORDS] = {"int", "double", "Main", "main", "switch", "void", "class", "default", "case", "break"};
int IndKeyW[KEYWORDS] = {INT, DOUBLE, MAIN, MAINFUNC, SWITCH, VOID, CLASS, DEFAULT, CASE, BREAK};

MemUK TScaner::getUk(){
    mem.line = line;
    mem.pos = pos;
    mem.uk = uk;
    return mem;
}

void TScaner::setUk(MemUK mem){
    this->uk = mem.uk;
    this->line = mem.line;
    this->pos = mem.pos;
}

int TScaner::compare(TLexem lex)
{
    for (int i = 0; i < KEYWORDS; i++)
        if (strcmp(lex, KeyWord[i]) == 0)
            return IndKeyW[i];
    return ID;
}

void TScaner::openFile(char *path){
    ifstream fin(path);

    if (!fin.is_open())
    {
        cout << "Incorrect path";
        return;
    }

    string line = "";
    int countBefore = 0, countAfter = 0;
    while(getline(fin, line))
    {
        countAfter = countBefore + line.length();
        int j = 0;
        for (int i = countBefore ; i < countAfter; i++)
        {
            text[i] = line[j];
            j++;
        }
        text[countAfter++] = '\n';
        countBefore = countAfter;
        //text += line;
        //text += '\n';
    }
    text += '\0';

    //fin.get(text, TEXTLEN);
    fin.close();
}

int TScaner::scaner(TLexem lex){
    int countNumber = 0;
    int i = 0;
    for (i = 0; i < MAXLEN; i++)
        lex[i] = 0;
    i = 0;

    //Ð¸Ð³Ð½Ð¾Ñ€Ð¸Ñ€ÑƒÐµÐ¼Ñ‹Ðµ ÑÐ¸Ð¼Ð²Ð¾Ð»Ñ‹
    begin: while ((text[uk] == ' ') || (text[uk] == '\n') || (text[uk] == '\t'))
    {
        if (text[uk] == '\n')
        {
            line++;
            pos = 0;
        }
        else if (text[uk] == '\t')
            pos+=4;
        else
            pos++;
        uk++;
    }

    if (text[uk] == '\0')
    {
        lex[0] = '#';
        return END;
    }
    else if ((text[uk] >= 'a' && text[uk] <= 'z') || (text[uk] >= 'A' && text[uk] <= 'Z'))
    {
        lex[i++] = text[uk++];
        countNumber++;
        while ((text[uk] >= 'a' && text[uk] <= 'z') ||
            (text[uk] >= 'A' && text[uk] <= 'Z') ||
            (text[uk] >= '0' && text[uk] <= '9'))
        {
            if (countNumber >= MAXLEN)
                uk++;
            else
                lex[i++] = text[uk++];
            countNumber++;
        }
        if (countNumber >= MAXLEN)
            return ERROR;
        return compare(lex);
    }
    else if (text[uk] >= '1' && text[uk] <= '9')
    {
        lex[i++] = text[uk++];
        countNumber++;
        while (text[uk] >= '0' && text[uk] <= '9')
        {
            if (countNumber >= NUMLEN)
                uk++;
            else
                lex[i++] = text[uk++];
            countNumber++;
        }
        if (countNumber >= NUMLEN)
            return ERROR;

        return CONSTINT10;
    }
    else if (text[uk] == '0')
    {
        lex[i++] = text[uk++];
        countNumber++;
        if (text[uk] == 'x' || text[uk] == 'X')
        {
            lex[i++] = text[uk++];
            countNumber++;
            while ((text[uk] >= '0' && text[uk] <= '9') ||
                   (text[uk] >= 'a' && text[uk] <= 'e') ||
                   (text[uk] >= 'A' && text[uk] <= 'E'))
            {
                if (countNumber >= NUMLEN)
                    uk++;
                else
                    lex[i++] = text[uk++];
                countNumber++;
            }
            if (lex[i-1] == 'X' || lex[i-1] == 'x')
                return ERROR;
            if (countNumber >= NUMLEN)
                return ERROR;
            return CONSTINT16;
        }

        while (text[uk] >= '0' && text[uk] <= '7')
            lex[i++] = text[uk++];
        return CONSTINT8;
    }
    else if (text[uk] == '\'')
    {
        lex[i++] = text[uk++];
        if ((text[uk] >= 'a' && text[uk] <= 'z') ||
                (text[uk] >= 'A' && text[uk] <= 'Z'))
            lex[i++] = text[uk++];
        else return ERROR;
        if (text[uk] == '\'')
            lex[i++] = text[uk++];
        else return ERROR;
        return CONSTCHAR;
    }
    else if (text[uk] == '+')
    {
        lex[i++] = text[uk++];
        if (text[uk] == '+')
        {
            lex[i++] = text[uk++];
            return INCREMENT;
        }
        return PLUS;
    }
    else if (text[uk] == '-')
    {
        lex[i++] = text[uk++];
        if (text[uk] == '-')
        {
            lex[i++] = text[uk++];
            return DECREMENT;
        }
        return MINUS;
    }
    else if (text[uk] == '*')
    {
        lex[i++] = text[uk++];
        return MULT;
    }
    else if (text[uk] == '/')
    {
        //ÐºÐ¾Ð¼Ð¼ÐµÐ½Ñ‚Ð°Ñ€Ð¸Ð¹
        if (uk < TEXTLEN && text[uk + 1] == '/')
        {
            uk++;
            while(text[uk] != '\n')
                uk++;
            if (text[uk] == '\0')
                return END;
            else goto begin;
        }
        //Ð´ÐµÐ»ÐµÐ½Ð¸Ðµ
        else
            lex[i++] = text[uk++];
        return DIV;
    }
    else if (text[uk] == '%')
    {
        lex[i++] = text[uk++];
        return PERCENT;
    }
    else if (text[uk] == '(')
    {
        lex[i++] = text[uk++];
        return OPENBRACKET;
    }
    else if (text[uk] == ')')
    {
        lex[i++] = text[uk++];
        return CLOSEBRACKET;
    }
    else if (text[uk] == '{')
    {
        lex[i++] = text[uk++];
        return OPENBRACE;
    }
    else if (text[uk] == '}')
    {
        lex[i++] = text[uk++];
        return CLOSEBRACE;
    }
    else if (text[uk] == ';')
    {
        lex[i++] = text[uk++];
        return SEMICOLON;
    }
    else if (text[uk] == ':')
    {
        lex[i++] = text[uk++];
        return COLON;
    }
    else if (text[uk] == '=')
    {
        lex[i++] = text[uk++];
        return ASSIGN;
    }
    else if (text[uk] == ',')
    {
        lex[i++] = text[uk++];
        return COMMA;
    }
    else {
        uk++;
        return ERROR;
    }
}

void TScaner::printError(char *err, TLexem l)
{
    cout << err << ". Îáíàðóæåíî " << l << " Ñòðîêà: " << line << endl;
    exit(1);
}

void TScaner::errMsg(char *err, TLexem l)
{
    cout << err << ": " << l << " Ñòðîêà: " << line << endl;
}
