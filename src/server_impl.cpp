#include "server_impl.h"

#include "http_request_impl.h"
#include "http_response_impl.h"
#include "handler_thread.h"

#include <logger_factory.h>

#include <QByteArray>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QRegularExpressionMatch>
#include <QThreadPool>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

LOGGER(ServerImpl);

static QString generateSocketUUID()
{
    static uint64_t num = 0;
    return QString("Socket %1").arg(++num);
}

ServerImpl::ServerImpl(QObject *parent) : QTcpServer(parent)
{
  connect(this, SIGNAL(newConnection()), this, SLOT(connection()), Qt::QueuedConnection);
  connect(this, &QTcpServer::acceptError, [this](){
      logger.log(ad::LogLevel::Error, this->errorString());
  });
}

ServerImpl::~ServerImpl()
{
    stop();
}

bool ServerImpl::begin(int port)
{
    logger.log(ad::LogLevel::Info, QString("Starting listening to port %1").arg(port));
    if (listen(QHostAddress::Any, port))
    {
        logger.log(ad::LogLevel::Info, QString("Server is listening to port %1").arg(port));
        return true;
    }
    else
    {
        logger.log(ad::LogLevel::Info, QString("Failed to listen to port %1").arg(port));
        return false;
    }
}
void ServerImpl::stop()
{
    logger.log(ad::LogLevel::Info, "Stopping the server");
    QTcpServer::close();
}

void ServerImpl::connection()
{
    logger.log(ad::LogLevel::Debug, "A new connection established");
    while (hasPendingConnections()) {
        logger.log(ad::LogLevel::Debug, "Incoming connection");
        QTcpSocket* socket = nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(handleQuery()));
        connect(socket, &QTcpSocket::disconnected, [=]() {
            logger.log(ad::LogLevel::Debug, QString("%1 disconnected").arg(__connections[socket]));
        });
        __connections.insert(socket, generateSocketUUID());
        logger.log(ad::LogLevel::Debug, QString("%1 connected").arg(__connections[socket]));
    }
}

void ServerImpl::handleQuery()
{
    QTcpSocket* source = qobject_cast<QTcpSocket*>(sender());
    if(source == nullptr)
        return;

    logger.log(ad::LogLevel::Debug, QString("Incoming query from [%1]").arg(__connections[source]));
    (new HandlerThread(*this, *source))->start();
}

void ServerImpl::addEndpoint(QRegularExpression const &endpoint, ad::http::RequestMethod method, handler_t handler)
{ 
    logger.log(ad::LogLevel::Info, QString("Registering new endpoint:\n\tmethod: %1\n\ttarget pattern: %2")
                                    .arg(QString(ad::http::methodVerbose(method)).toUpper())
                                    .arg(endpoint.pattern()));
    for(QList<endpoint_t>::iterator it = __handlers.begin(); it != __handlers.end(); ++it)
    {
        if(endpoint != it->first)
            continue;

        it->second[method] = handler;
        logger.log(ad::LogLevel::Info, "The endpoint already exists");
        return;
    }

    __handlers.push_back(endpoint_t(endpoint, QMap<ad::http::RequestMethod, handler_t> {{method, handler}}));
}
