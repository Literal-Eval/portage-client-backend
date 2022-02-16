#include "input.h"

Input::Input(FTP* ftp) : QObject(nullptr)
{
    this->ftp = ftp;
}

void Input::listen()
{
    bool isRunning {true};
    std::string input;

    while (isRunning) {
        qInfo() << "Client: ";
        std::cin >> input;
        input = QString::fromStdString(input).toUpper().toStdString();

        if (input == "QUIT") {
            ftp->newCommand("QUIT");
            isRunning = false;
        }

        else {
            ftp->newCommand(input);
        }
    }
}
