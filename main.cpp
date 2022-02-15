#include <QCoreApplication>
#include <QThread>

#include "ftp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread ftpThread;

    FTP ftp;
    bool isRunning {true};
    std::string input;

    ftp.moveToThread(&ftpThread);
    ftpThread.start();

    while (isRunning) {
        qInfo() << "Client: ";
        std::cin >> input;
        input = QString::fromStdString(input).toUpper().toStdString();

        if (input == "QUIT") {
            ftp.newCommand("QUIT");
            isRunning = false;
        }

        else {
            ftp.newCommand(input);
        }
    }

    ftpThread.quit();
    exit(0);
    return a.exec();
}
