#ifndef FTP_H
#define FTP_H

#include <QObject>

#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QFile>

#include <iostream>

class FTP : public QObject
{
    Q_OBJECT

    const QHostAddress ftp_ip {"192.168.159.15"};
    const qint32 ftp_port {2121};

    const QHostAddress server_ip {"192.168.159.60"};
    const qint32 server_port {6969};

    QTcpSocket controlSocket;
    QTcpServer dataServer;
    QTcpSocket* dataSocket {nullptr};

    QFile currentFile;

    bool isRunning {false};
    bool nextPassiveCmd {false};

public:
    explicit FTP(QObject *parent = nullptr);
    ~FTP();

    void newCommand(QString cmd);
    void setPassiveMode(QString res);
    void closeSockets();

signals:

public slots:
    void controlReadyRead();
    void serverNewConnection();
    void dataReadyRead();
};

#endif // FTP_H
