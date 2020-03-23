#ifndef PETCLIENT_H
#define PETCLIENT_H

#include <QWidget>

class PetClient : public QWidget
{
    Q_OBJECT

public:
    PetClient(QWidget *parent = nullptr);
    ~PetClient();
};
#endif // PETCLIENT_H
