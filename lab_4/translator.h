#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "word.h"

class Translator
{
public:
    Translator();
    ~Translator();
    void translate(QString pathFileIn);

private:
    bool loadFiles(); //Загрузка словарей из файлов(словари запиисываются в строки для файлов)
    void parserInVectors(ushort lang, QString file); //Парсерим файлы в векторы(в словари)
    QVector<Word> splitWords(QVector<Word> ___words);//Разбиваем слова с разными окончаниями и формами. Указываем, какой словарь разбиваем(то есть избавляемся от слешей в словаре)
    void outToFileAppend(QString text, QString fileName);//Вывести текст в указанный файл(дописать в файл)
    void outToFile(QString text, QString fileName); //Вывести текст в указанный файл(переписать файл)
    void removeFile(QString fileName); //Удалить указанный файл
    QVector<QString> textToWords(QString text); // разбивает текст на слова. Возрвращает вектор этих слов.
    QVector<uint> getVectorId(QVector<QString> words, QVector<Word> ___words); //Принимает вектор слов и словарь. И возвращает вектор их id.
    QVector<QString> idToVectorStr(QVector<uint> ids, QVector<Word> ___words); //Принимает айдишники слов и словарь языка в который хотим перевести
    QVector<QString> syntacticAnalysisEng(QVector<uint> ids);//Синтаксический анализ для английского текста(принимает и возвращает вектор id текста)
    QVector<QString> syntacticAnalysisFre(QVector<uint> ids);//Синтаксический анализ для французского текста(принимает и возвращает id слов)


private:
    //Словари
    QVector<Word> rusWords;
    QVector<Word> engWords;
    QVector<Word> freWords;

    //Файлы
    QString file_rusWords;
    QString file_engWords;
    QString file_freWords;
};

#endif // TRANSLATOR_H
