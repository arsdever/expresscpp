#pragma once

#include <QTcpServer>
#include <QFunctionPointer>
#include <QMap>
#include <QList>
#include <QPair>
#include <QRegularExpression>

#include "request.h"
#include "response.h"

class Server : public QTcpServer {
  Q_OBJECT

public:
  using request_t = ad::http::HttpRequest;
  using response_t = ad::http::HttpResponse;
  using handler_t = void (*) (request_t&, response_t&);
  using endpoint_t = QPair<QRegularExpression, QMap<ad::http::RequestMethod, handler_t> >;

public:
  Server(QObject *parent = nullptr);
  void begin();

  response_t handleRequest(request_t &request);

  void addEndpoint(QRegularExpression const &endpoint, ad::http::RequestMethod method, handler_t handler);
  inline void GET(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Get, handler); }
  inline void HEAD(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Head, handler); }
  inline void POST(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Post, handler); }
  inline void PUT(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Put, handler); }
  inline void DELETE(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Delete, handler); }
  inline void CONNECT(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Connect, handler); }
  inline void OPTIONS(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Options, handler); }
  inline void TRACE(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Trace, handler); }
  inline void PATCH(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Patch, handler); }

private slots:
  void connection();
  void handleQuery();

private:
  QList<QTcpSocket*> __connections;
  QList<endpoint_t>  __handlers;
};
