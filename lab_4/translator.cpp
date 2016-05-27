#include "translator.h"

Translator::Translator()
{
    loadFiles();
    parserInVectors(ENG_WORD, file_engWords);
    parserInVectors(RUS_WORD, file_rusWords);
    parserInVectors(FRE_WORD, file_freWords);
    rusWords = splitWords(rusWords);
    engWords = splitWords(engWords);
    freWords = splitWords(freWords);
}

QVector<Word> Translator::splitWords(QVector<Word> ___words)
{
    //Разбиваем слова с разными окончаниями и формами.
    //Указываем, какой словарь разбиваем(то есть избавляемся от слешей в словаре)

    QVector<QString> words;
    QString new_word;
    for(int i(0); i<___words.size();++i)
    {
        new_word = "";
        QString str =  ___words[i].getWord(0);
        for(int j(0); j<str.length();++j)
        {
            if(str[j] != '/')
                new_word.append(str[j]);
            else
            {
                words.push_back(new_word);
                new_word = "";
            }
        }
        if(words.size() != 0)
        {
            words.push_back(new_word);
            new_word = "";
            ___words[i].clearWords();

            for(int k(0); k<words.size();++k)
                ___words[i].setWord(words[k]);

            words.clear();
        }
    }

    return ___words;
}

void Translator::outToFileAppend(QString text, QString fileName)
{
    QFile myFile(fileName);
    if(!myFile.open(QFile::Append | QFile::Text)){}
    QTextStream myText(&myFile);
    myText << text;
    myFile.flush();
    myFile.close();
}

void Translator::outToFile(QString text, QString fileName)
{
    QFile myFile(fileName);
    if(!myFile.open(QFile::WriteOnly | QFile::Text)){}
    QTextStream myText(&myFile);
    myText << text;
    myFile.flush();
    myFile.close();
}

void Translator::removeFile(QString fileName)
{
    QFile myFile(fileName);
    if(!myFile.open(QFile::ReadOnly | QFile::Text)){    }
    myFile.remove();
    myFile.close();
}

bool Translator::loadFiles()
{
    //Загрузка словарей
    //Пути к файлам
    QString pathRusWords = "rusWords.txt";
    QString pathEngWords = "engWords.txt";
    QString pathFreWords = "freWords.txt";

    //Здесь будут хранится файлы
    file_rusWords = "NONE";
    file_engWords = "NONE";
    file_freWords = "NONE";

    QString path = pathRusWords;
    int num = 0;
    while(num<3)
    {
        QFile myFile(path);
        if(!myFile.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() <<"Не удалось открыть файл";
        }
        else
        {
            QTextStream myText(&myFile);
            if(num == 0)
            {
                file_rusWords = myText.readAll();
                path = pathEngWords;
            }
            if(num == 1)
            {
                file_engWords = myText.readAll();
                path = pathFreWords;
            }
            if(num == 2)
                file_freWords = myText.readAll();

            myFile.close();
        }
        num++;
    }

    if(file_rusWords != "NONE" && file_engWords != "NONE" && file_freWords != "NONE")
        return true;
    else
        return false;
}

void Translator::parserInVectors(ushort lang, QString file)
{
    //Парсерим словари в вектора
    for(int i(0); i<file.length(); ++i)
    {
        if(file[i] == ':')
        {
            ushort type = 0;
            if(file[i-1] == 'n') type = NOUN;
            else if(file[i-1] == 'a') type = ADJECTIVE;
            else if(file[i-1] == 'v') type = VERB;
            else if(file[i-1] == 'p') type = PRENOUN;
            else if(file[i-1] == 'r') type = PREPOSITION;
            else if(file[i-1] == 't') type = ARTICLE;

            int n = i+1;
            QString word;
            while(file[n] != '[')
            {
                word.append(file[n]);
                n++;
            }

            n++;
            QString id;
            while(file[n] != ']')
            {
                id.append(file[n]);
                n++;
            }

            Word new_word(id.toInt(),lang,type,word);
            switch(lang)
            {
            case ENG_WORD: this->engWords.push_back(new_word); break;
            case RUS_WORD: this->rusWords.push_back(new_word); break;
            case FRE_WORD: this->freWords.push_back(new_word); break;
            }
        }
    }
}

QVector<QString> Translator::textToWords(QString text)
{
    //Разбиваем текст на отдельные слова
    QVector<QString> words;
    QString word = "";
    foreach (QChar c, text)
    {
        if(c == ' ' || c == '\n')
        {
            words.push_back(word);
            word = "";
        }
        else
        {
            if(c != '.' && c != ',' && c != '!' && c != '?' && c != '\n')
                word.append(c);
        }
    }

    if(word != "")
        words.push_back(word);

    return words;
}

QVector<uint> Translator::getVectorId(QVector<QString> words, QVector<Word> ___words)
{
    //Теперь составляем вектор айдишников
    QVector<uint> ids;
    foreach (QString str, words)
        foreach (Word word, ___words)
            foreach (QString str_2, word.getWord())
                if(str == str_2)
                    ids.push_back(word.getId());

    return ids;
}

QVector<QString> Translator::idToVectorStr(QVector<uint> ids, QVector<Word> ___words)
{
    //Переводим айдишники в вектор слов нужного нам языка(выводим первое слово в языке, без синтаксической обработки)
    QVector<QString> words;
    foreach (uint id, ids)
        foreach (Word word, ___words)
            if(word.getId() == id)
                words.push_back(word.getWord(0));

    return words;
}

void Translator::translate(QString pathFileIn)
{
    QFile myFile(pathFileIn);
    if(!myFile.open(QFile::ReadOnly | QFile::Text))
    {

    }
    else
    {
        QTextStream myText(&myFile);
        QString text = myText.readAll();
        myFile.close();

        /*Переводим введенный текст в вектор Слов, затем получаем из него вектор-Id, потом этот вектор проходит
         *синтаксический анализ, и возвращает нам QVector<QString>, то есть предложение на английском(первый перевод)
         */
        QVector<QString> translate_words = syntacticAnalysisEng(getVectorId(textToWords(text),rusWords));
        //Дальше по айдишникам переводим в французский текст
        QVector<QString> translate_words_2 = idToVectorStr(getVectorId(translate_words,engWords),freWords);

        removeFile("out.txt");
        outToFileAppend("Intermediate translation:\n","out.txt");
        foreach (QString str, translate_words) {
            outToFileAppend(str,"out.txt");
            outToFileAppend(" ","out.txt");
        }

        outToFileAppend("\n\nFinal translation:\n","out.txt");
        foreach (QString str, translate_words_2) {
            outToFileAppend(str,"out.txt");
            outToFileAppend(" ","out.txt");
        }

        qDebug() <<"Your text is translated successfully!\n\nRefer to the file \"out.txt\" ....";
    }
}

QVector<QString> Translator::syntacticAnalysisEng(QVector<uint> ids)
{
    //Получаем вектор слов(Word) по айдишникам.
    QVector<Word> words;
    foreach (uint i, ids) {
        Word new_word(engWords[i].getId(),engWords[i].getLang(),
                      engWords[i].getType(),engWords[i].getWord());
        words.push_back(new_word);
    }

    //СИНТАКСИЧЕСКИЕ ПРАВИЛА

    //Расстановка артиклей
    //Если последнее слово существительное, а перед ним предлог at, ставим неопределнный артикль
    if(words[words.size()-1].getType() == NOUN && words[words.size()-2].getId() == 15)
    {
        Word new_word(1000000,ENG_WORD,ARTICLE,"a");
        words.insert(words.size()-1,new_word);
    }



    for(int i(0);i<words.size();++i)
    {

        //Если у предмет единственен и уникален в своем роде
        //begin
        for(int j(0);j<words[i].getWord().size();++j)
        {
            QString str = words[i].getWord(j);
            if(str == "street")
            {
                if(i==0)
                {
                    Word new_word(1000001,ENG_WORD,ARTICLE,"the");
                    words.insert(i,new_word);
                }
                else
                {
                    if(words[i-1].getWord(j) != "the")
                    {
                        Word new_word(1000001,ENG_WORD,ARTICLE,"the");
                        words.insert(i,new_word);
                    }
                }
            }
        }
        //end

        //Если перед существительным прилагательное, то ставим перед прилагательным артикль а
        //begin
        if(words[i].getType() == NOUN)
        {
            if(i!=0)
            {
                if(words[i-1].getType() == ADJECTIVE)
                {
                    if((i-1) != 0)
                    {
                        if( words[i-2].getWord(0) != "a")
                        {
                            Word new_word(1000000,ENG_WORD,ARTICLE,"a");
                            words.insert(i-1,new_word);
                        }
                    }
                    else
                    {
                        Word new_word(1000000,ENG_WORD,ARTICLE,"a");
                        words.insert(i-1,new_word);
                    }
                }
            }
        }
        //end

        //Если первое слово существительное, то ставим артикль The
        //begin
        if(words[0].getType() == NOUN)
        {
            Word new_word(1000001,ENG_WORD,ARTICLE,"the");
            words.insert(0,new_word);
        }
        //end
    }



    //Получаем вектор строк и возвращаем его
    QVector<QString> words_total;
    foreach (Word word, words) {
        words_total.push_back(word.getWord(0));
    }

    return words_total;
}

QVector<QString> Translator::syntacticAnalysisFre(QVector<uint> ids)
{
    //Получаем вектор слов(Word) по айдишникам.
    QVector<Word> words;
    foreach (uint i, ids) {
        Word new_word(freWords[i].getId(),freWords[i].getLang(),
                      freWords[i].getType(),freWords[i].getWord());
        words.push_back(new_word);
    }

    //СИНТАКСИЧЕСКИЕ ПРАВИЛА

    //Расстановка артиклей
    //Если последнее слово существительное, а перед ним предлог at, ставим неопределнный артикль
    if(words[words.size()-1].getType() == NOUN && words[words.size()-2].getId() == 15)
    {
        Word new_word(1000000,ENG_WORD,ARTICLE,"un");
        words.insert(words.size()-1,new_word);
    }


    //Получаем вектор строк и возвращаем его
    QVector<QString> words_total;
    foreach (Word word, words) {
        words_total.push_back(word.getWord(0));
    }

    return words_total;
}

Translator::~Translator(){}
