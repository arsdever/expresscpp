#include "server_impl.h"

#include <QByteArray>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "http_request_impl.h"
#include "http_response_impl.h"

ServerImpl::ServerImpl(QObject *parent) : QTcpServer(parent)
{
  connect(this, SIGNAL(newConnection()), this, SLOT(connection()));
}

void ServerImpl::begin(int port) { listen(QHostAddress::Any, port); }
void ServerImpl::stop() { QTcpServer::close(); }

void ServerImpl::connection()
{
    QTcpSocket* socket = nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleQuery()));
    __connections.push_back(socket);
}

void ServerImpl::handleQuery()
{
    QTcpSocket* source = qobject_cast<QTcpSocket*>(sender());
    if(source == nullptr)
        return;

    request_t *request = new ad::http::HttpRequestImpl();
    request->deserialize(source->readAll());
    response_t *res = handleRequest(*request);
    delete request;
    source->write(res->serialize());
    delete res;
}

ServerImpl::response_t *ServerImpl::handleRequest(request_t &request)
{
    for(QList<endpoint_t>::const_iterator it = __handlers.cbegin(); it != __handlers.cend(); ++it)
    {
        if(!QString(request.target()).contains(it->first))
            continue;

        if(!it->second.contains(request.method()))
            continue;

        response_t *response = new ad::http::HttpResponseImpl(ad::http::HttpVersion::Http1_1, ad::http::ResponseStatus::Ok);
        it->second[request.method()](request, *response);
        return response;
    }
    return new ad::http::HttpResponseImpl(ad::http::HttpVersion::Http1_1, ad::http::ResponseStatus::MethodNotAllowed);
}

void ServerImpl::addEndpoint(QRegularExpression const &endpoint, ad::http::RequestMethod method, handler_t handler)
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
