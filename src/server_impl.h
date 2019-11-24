#pragma once

#include <QTcpServer>
#include <QFunctionPointer>
#include <QMap>
#include <QList>
#include <QPair>
#include <QRegularExpression>

#include <http_request.h>
#include <http_response.h>
#include "server.h"

class ServerImpl : public QTcpServer, public Server {
  Q_OBJECT

public:
  using request_t = ad::http::HttpRequest;
  using response_t = ad::http::HttpResponse;
  using handler_t = void (*) (request_t&, response_t&);
  using endpoint_t = QPair<QRegularExpression, QMap<ad::http::RequestMethod, handler_t> >;

public:
  ServerImpl(QObject *parent = nullptr);
  
  void begin(int port = 8080) override;
  void stop() override;
  void addEndpoint(QRegularExpression const &endpoint, ad::http::RequestMethod method, handler_t handler) override;

  response_t *handleRequest(request_t &request);

private slots:
  void connection();
  void handleQuery();

private:
  QList<QTcpSocket*> __connections;
  QList<endpoint_t>  __handlers;
};
