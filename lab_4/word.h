#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QVector>

#define ENG_WORD 1
#define RUS_WORD 2
#define FRE_WORD 3

#define NOUN 1
#define PRENOUN 2
#define ADJECTIVE 3
#define VERB 4
#define PREPOSITION 5
#define ARTICLE 6

class Word
{
    typedef unsigned int uint;
    typedef unsigned short ushort;

public:
    Word(uint id, ushort lang, ushort type, QString word);
    Word(uint id, ushort lang, ushort type, QVector<QString> words);
    Word();
    ~Word();

    void setId(uint id);
    void setLang(ushort lang);
    void setType(ushort type);
    void setWord(QString word);
    void clearWords();
    uint getId();
    ushort getLang();
    ushort getType();
    QString getWord(uint num);
    QVector<QString> getWord();


private:
    uint id;
    ushort lang;
    ushort type;
    QVector<QString> word;
};

#endif // WORD_H
