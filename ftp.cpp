#include "ftp.h"

FTP::FTP(QObject *parent) : QObject(parent)
{
    QObject::connect(&controlSocket, &QTcpSocket::readyRead, this, &FTP::controlReadyRead);
    QObject::connect(&dataServer, &QTcpServer::newConnection, this, &FTP::serverNewConnection);

    currentFile.setFileName("out.txt");
    currentFile.open(QIODevice::Append);

    if (!currentFile.isOpen()) {
        std::cout << "Error creating file\n";
    }
}

void FTP::newCommand(QString cmd) {
    if (cmd.toUpper() == "START") {

        QObject::connect(&controlSocket, &QTcpSocket::connected, [&] () {
            std::cout << "Socket connected\n";
        });

        QObject::connect(&controlSocket, &QTcpSocket::errorOccurred,
                         [] (QAbstractSocket::SocketError error)
        {
            qDebug() << error;
        });

        controlSocket.connectToHost(ftp_ip, ftp_port);
        controlSocket.waitForConnected();

        std::string userName;
        std::string password;

        qInfo() << "USER: ";
        std::cin >> userName;

        controlSocket.write(("USER " + userName + "\r\n").c_str());
        controlSocket.waitForBytesWritten();
        controlSocket.waitForReadyRead();
        controlReadyRead();

        qInfo() << "PASS: ";
        std::cin >> password;

        controlSocket.write(("PASS " + password + "\r\n").c_str());
    }

    else if (cmd.toUpper() == "QUIT") {
        controlSocket.write("QUIT\r\n");
        closeSockets();
    }

    else if (cmd.toUpper() == "PASV") {
        nextPassiveCmd = true;
        controlSocket.write("PASV\r\n");
    }

    else {
        if (cmd != "\n")
        controlSocket.write((cmd + "\r\n").toStdString().c_str());
    }

    controlSocket.waitForBytesWritten();
    controlSocket.waitForReadyRead();
}

void FTP::controlReadyRead() {
    QString res;
    while (controlSocket.bytesAvailable()) {
        res += controlSocket.readAll();
    }

    if (nextPassiveCmd) {
        setPassiveMode(res);
        nextPassiveCmd = false;
    }

    if (res == "") return;
    qInfo() << "Server: " << res;
}

void FTP::serverNewConnection() {
    dataSocket = dataServer.nextPendingConnection();
    QObject::connect(dataSocket, &QTcpSocket::readyRead, this, &FTP::dataReadyRead);
}

void FTP::setPassiveMode(QString res) {
    int startPos {0};

    for (int i {0}; i < res.length(); i++) {
        if (res[i] == '(') {
            startPos = i + 1;
        }
    }

    QString ipData = res.mid(startPos);
    ipData = ipData.mid(0, ipData.length() - 3);
//    qInfo() << ipData;

    QStringList ipSplit = ipData.split(',');

    QHostAddress remoteIp {ipSplit[0] + '.'
                            + ipSplit[1] + '.'
                            + ipSplit[2] + '.'
                            + ipSplit[3]};
    int remotePort {(ipSplit[4].toInt() << 8) | ipSplit[5].toInt()};

    dataSocket = new QTcpSocket;
    dataSocket->connectToHost(remoteIp, remotePort);

    QObject::connect(dataSocket, &QTcpSocket::connected, [&] () {
        qInfo() << "Data socket connected with " << dataSocket->localAddress()
                << " at port " << dataSocket->localPort();

//        dataSocket->waitForReadyRead();
    });

    QObject::connect(dataSocket, &QTcpSocket::readyRead, this, &FTP::dataReadyRead);
}

void FTP::dataReadyRead() {
    while (dataSocket->bytesAvailable()) {
        currentFile.write(dataSocket->readAll());
    }

    qInfo() << "Reading from data socket";
    controlReadyRead();
}

void FTP::closeSockets() {
    if (controlSocket.isOpen())     controlSocket.close();
    if (dataServer.isListening())   dataServer.close();
    if (dataSocket)                 dataSocket->close();

    currentFile.close();
}

FTP::~FTP() {
    closeSockets();
}
