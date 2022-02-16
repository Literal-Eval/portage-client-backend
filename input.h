#ifndef INPUT_H
#define INPUT_H

#include <QObject>

#include "ftp.h"
#include <iostream>

class Input : public QObject
{
    Q_OBJECT

    FTP* ftp;

public:
    explicit Input(FTP* ftp);

    void listen();

signals:

};

#endif // INPUT_H
