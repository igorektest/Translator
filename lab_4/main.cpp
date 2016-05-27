#include <QCoreApplication>
#include "translator.h"

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    Translator translator;
    translator.translate("in.txt");

    return a.exec();
}
