#include <QCoreApplication>
#include <QThread>

#include "ftp.h"
#include "input.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread ftpThread;
    QThread inputThread;

    FTP ftp;
    ftp.moveToThread(&ftpThread);
    ftpThread.start();

    Input input {&ftp};
    input.moveToThread(&inputThread);
    inputThread.start();

    input.listen();

    ftpThread.quit();
    inputThread.quit();
    exit(0);
    return a.exec();
}
