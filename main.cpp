#include <QCoreApplication>
#include <QThread>

#include <QConsoleListener>

#include "ftp.h"
#include "input.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread ftpThread;

    FTP ftp;

    QConsoleListener console;
    QObject::connect(&console, &QConsoleListener::newLine,
    [&](const QString &strNewLine) {

        if (strNewLine.toUpper() == "QUIT") {
            ftp.newCommand("QUIT");
            exit(0);
        }

        else {
            ftp.newCommand(strNewLine);
        }

        std::cout << "Client: ";
    });

    return a.exec();
}
