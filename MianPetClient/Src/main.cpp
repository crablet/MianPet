#include "PetClient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PetClient w;
    w.hide();
    return a.exec();
}
