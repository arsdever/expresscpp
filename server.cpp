#include "server.h"

#include <QByteArray>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

Server::Server(QObject *parent) : QTcpServer(parent), __logger("server.log")
{
  connect(this, SIGNAL(newConnection()), this, SLOT(connection()));
  __logger.setLogLevel(ad::LogLevel::Trace);
}

void Server::begin() { listen(QHostAddress::Any, 8008); }

void Server::connection()
{
    logger().log(ad::LogLevel::Info, "Incoming connection");
    QTcpSocket* socket = nextPendingConnection();
    logger().log(ad::LogLevel::Debug, QString("Source %1").arg((uint64_t)socket));
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleQuery()));
    __connections.push_back(socket);
}

void Server::handleQuery()
{
    logger().log(ad::LogLevel::Debug, "Handling query");
    QTcpSocket* source = qobject_cast<QTcpSocket*>(sender());
    logger().log(ad::LogLevel::Debug, QString("Source %1").arg((uint64_t)source));
    if(source == nullptr)
        return;

    request_t request;
    request.deserialize(source->readAll());
    response_t res = handleRequest(request);
    source->write(res.serialize());
}

Server::response_t Server::handleRequest(request_t &request)
{
    for(QList<endpoint_t>::const_iterator it = __handlers.cbegin(); it != __handlers.cend(); ++it)
    {
        if(!QString(request.target()).contains(it->first))
            continue;

        if(!it->second.contains(request.method()))
            continue;

        response_t response(ad::http::HttpVersion::Http1_1, ad::http::ResponseStatus::Ok);
        it->second[request.method()](request, response);
        return response;
    }
    return response_t(ad::http::HttpVersion::Http1_1, ad::http::ResponseStatus::MethodNotAllowed);
}

void Server::addEndpoint(QRegularExpression const &endpoint, ad::http::RequestMethod method, handler_t handler)
{   
    for(QList<endpoint_t>::iterator it = __handlers.begin(); it != __handlers.end(); ++it)
    {
        if(endpoint != it->first)
            continue;

        it->second[method] = handler;
        return;
    }

    __handlers.push_back(endpoint_t(endpoint, QMap<ad::http::RequestMethod, handler_t> {{method, handler}}));
}
