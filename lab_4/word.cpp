#include "word.h"

Word::Word(uint id, ushort lang, ushort type, QString word)
{
    this->id = id;
    this->lang = lang;
    this->type = type;
    this->word.push_back(word);
}

Word::Word(uint id, ushort lang, ushort type, QVector<QString> words)
{
    this->id = id;
    this->lang = lang;
    this->type = type;
    this->word = words;
}

Word::Word()
{
    this->id = 999999;
    this->lang = 0;
    this->type = 0;
    this->word.push_back("NONE");
}

Word::~Word()
{

}

void Word::setId(uint id)
{
    this->id = id;
}

void Word::setLang(ushort lang)
{
    this->lang = lang;
}

void Word::setType(ushort type)
{
    this->type = type;
}

void Word::setWord(QString word)
{
    this->word.push_back(word);
}

unsigned int Word::getId()
{
    return id;
}

unsigned short Word::getLang()
{
    return lang;
}

unsigned short Word::getType()
{
    return type;
}

QString Word::getWord(uint num)
{
    return word[num];
}

QVector<QString> Word::getWord()
{
    return word;
}

void Word::clearWords()
{
    word.clear();
}
