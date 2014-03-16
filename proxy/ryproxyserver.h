#ifndef RYPROXYSERVER_H
#define RYPROXYSERVER_H

#include <QtCore>
#include <QTcpServer>
#include "ryconnection.h"


class RyProxyServer : public QTcpServer
{
        Q_OBJECT
    public:
        static RyProxyServer* instance();
        explicit RyProxyServer();
        ~RyProxyServer();
        void close();

        int maxOfSocket();
        void setMaxSocket(int max);

        quint64 nextPipeId();

        // TODO
        static bool isCurrentProxy(QStringList& hostAndPor){
            if(hostAndPor.size() == 2){
                QString host = hostAndPor.at(0);
                QString port = hostAndPor.at(1);
                qDebug()<<host<<port;
                if((host == "127.0.0.1" ||
                        host.toLower() == "localhost") &&
                        port == "8889"

                        ){
                    return true;
                }
            }
            return false;
        }

    signals:
        void pipeBegin(RyPipeData_ptr);
        void pipeComplete(RyPipeData_ptr);
        void pipeError(RyPipeData_ptr);
    public slots:
        void cacheSocket(QString address,quint16 port,QTcpSocket* socket);
        QTcpSocket* getSocket(QString address,quint16 port,bool* isFromCache,QThread* _desThread);


    protected:
        void incomingConnection(int handle);

    private:
        QMutex connectionOpMutex;
        QMutex _socketsOpMutex;

        bool isStoping;
        int _maxOfSocket;
        //sockets
        // cache for reuse
        QMultiMap<QString,QTcpSocket*> _cachedSockets;
        QMultiMap<QString,QTcpSocket*> _liveSockets;
        //connections
        //TODO  should has a timeout to kill connections
        //      when those idle to much time
        QList<RyConnection*> _connections;

        QMap<RyConnection*,QThread*> _threads;

        RyConnection *_getConnection(int handle);

        quint64 _lastPipeId;
        quint64 _lastConnectionId;

        static RyProxyServer* _instance;


    private slots:
        void onConnectionIdleTimeout();
        void onPipeBegin(RyPipeData_ptr);
        void onPipeComplete(RyPipeData_ptr);
        void onPipeError(RyPipeData_ptr);
        void onThreadTerminated();
        void onConnectionClosed();



};

#endif // RYPROXYSERVER_H
