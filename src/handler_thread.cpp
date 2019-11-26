#include "handler_thread.h"

#include <logger_factory.h>
#include "http_request_impl.h"
#include "http_response_impl.h"

#include <QAbstractSocket>

uint64_t ServerImpl::HandlerThread::s__serial_number = 0;

ServerImpl::HandlerThread::HandlerThread(ServerImpl &parent, QAbstractSocket &socket)
	: __local_id(QString("HandlerThread %1").arg(++s__serial_number))
	, __logger(ad::LoggerFactory::getLoggerFor(__local_id))
	, __parent(parent)
    , __socket(socket)
	, __request(new ad::http::HttpRequestImpl())
	, __response(new ad::http::HttpResponseImpl(ad::http::HttpVersion::Http1_1, ad::http::ResponseStatus::Ok))
    , __completed(false)
{
    __socket.setParent(nullptr);
    __socket.moveToThread(this);
    QByteArray data = __socket.readAll();
    __request->deserialize(data);
	__logger.log(ad::LogLevel::Debug, "Handling thread created");
    __logger.log(ad::LogLevel::Debug, QString("Request is:\n%1").arg(QString(data)));
    connect(this, &QThread::finished, this, &QThread::deleteLater);
}

ServerImpl::HandlerThread::~HandlerThread()
{
	__logger.log(ad::LogLevel::Debug, "Handling thread's being released");
	ad::LoggerFactory::global().releaseLogger(__local_id);
}

void ServerImpl::HandlerThread::run()
{
    __logger.log(ad::LogLevel::Info, QString("Looking for endpoint %1: %2")
                                    .arg(QString(ad::http::methodVerbose(__request->method())).toUpper())
                                    .arg(QString(__request->target())));
    for(QList<endpoint_t>::const_iterator it = __parent.__handlers.cbegin(); it != __parent.__handlers.cend(); ++it)
    {
        QRegularExpressionMatch match = it->first.match(QString(__request->target()));
        if (!match.hasMatch())
            continue;

        __logger.log(ad::LogLevel::Debug, QString("The matches are: [%1]")
                                        .arg(match.capturedTexts().join(", ")));

        __logger.log(ad::LogLevel::Info, QString("Found endpoint matches pattern \"%1\"").arg(it->first.pattern()));
        if (!it->second.contains(__request->method()))
            continue;

        __logger.log(ad::LogLevel::Info, QString("Handling request %1: %2").arg(ad::http::methodVerbose(__request->method())).arg(QString(__request->target())));
        it->second[__request->method()](*__request, *__response);
    }

    connect(&__socket, &QAbstractSocket::bytesWritten, [this]() {
        this->__completed = true;
    });
    QByteArray result = __response->serialize();
    __logger.log(ad::LogLevel::Debug, QString("Writing the response and sending back:\n%1").arg(QString(result)));
    __socket.write(result);
    __socket.flush();

    while (!__completed)
    {
        usleep(100);
    }
}