#pragma once

#include <http/http.h>
#include <http_request.h>
#include <http_response.h>

class QRegularExpression;
template <typename t1, typename t2>
class QPair;
template <typename k, typename v>
class QMap;

class Server{
public:
  using request_t = ad::http::HttpRequest;
  using response_t = ad::http::HttpResponse;
  using handler_t = void (*) (request_t&, response_t&);
  using endpoint_t = QPair<QRegularExpression, QMap<ad::http::RequestMethod, handler_t> >;

public:
  virtual bool begin(int port = 8080) = 0;
  virtual void stop() = 0;
  virtual void addEndpoint(QRegularExpression const &endpoint, ad::http::RequestMethod method, handler_t handler) = 0;
  
  inline void GET(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Get, handler); }
  inline void HEAD(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Head, handler); }
  inline void POST(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Post, handler); }
  inline void PUT(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Put, handler); }
  inline void DELETE(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Delete, handler); }
  inline void CONNECT(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Connect, handler); }
  inline void OPTIONS(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Options, handler); }
  inline void TRACE(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Trace, handler); }
  inline void PATCH(QRegularExpression const &endpoint, handler_t handler) { addEndpoint(endpoint, ad::http::RequestMethod::Patch, handler); }
};
