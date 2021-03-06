#ifndef DEF_NETWORK_H
#define DEF_NETWORK_H

#include <QtWidgets>
#include <QtNetwork>

#define SERVER_PORT 23090

class Server : public QWidget
{
    Q_OBJECT

    public:
        Server();

    private slots:
        void newConnection();
        void dataReceived();
        void clientDisconnected();

    private:
        QTcpServer* server;
        QList<QTcpSocket*> clients;
        quint16 messageSize;
};

#endif
