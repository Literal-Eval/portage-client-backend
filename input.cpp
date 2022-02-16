#include "input.h"

Input::Input(FTP* ftp) : QObject(nullptr)
{
    this->ftp = ftp;
}

void Input::ioCheck() {
    std::cout << "Enter from Input ";
    std::string name;
    std::cin >> name;
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
            ftp->newCommand(QString::fromStdString(input));
        }
    }
}
