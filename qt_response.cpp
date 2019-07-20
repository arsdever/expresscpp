#include "qt_response.h"
#include <QDebug>

namespace ad
{
    namespace http
    {
        HttpResponse::HttpResponse(HttpVersion version, ResponseStatus status, QByteArray body)
            : __http_version(version)
            , __status(status)
            , __body(body)
        {
            addHeader("Content-Type", contentTypeVerbose(ContentType::TXT));
            addHeader("Content-Length", QByteArray::number(body.size()));
        }

        HttpResponse::~HttpResponse() {}

        QByteArray HttpResponse::serialize() const
        {
            QByteArray result;
            result += QByteArray(httpVersionVerbose(httpVersion()));
            result.push_back(' ');
            result += QByteArray::number((int)status());
            result.push_back(' ');
            result += QByteArray(reasonPhraseVerbose(status())) + "\r\n";
            for(QByteArray h : headers().keys())
                result += h + ": " + header(h) + "\r\n";

            result += "\r\n";
            result += body();
            return result;
        }

        void HttpResponse::deserialize(QByteArray const &data) {}
        void HttpResponse::setStatus(ResponseStatus status) { __status = status; }
        void HttpResponse::setHttpVersion(HttpVersion version) { __http_version = version; }
        void HttpResponse::addHeader(QByteArray const &key, QByteArray const &value) { __headers.insert(key, value); }
        void HttpResponse::setHeaders(QMap<QByteArray, QByteArray> const &headers) { __headers = headers; }
        void HttpResponse::setBody(QByteArray const &body, ContentType type)
        {
            __body = body;
            addHeader("Content-Type", contentTypeVerbose(type));
            addHeader("Content-Length", QByteArray::number(body.size()));
        }
        ResponseStatus HttpResponse::status() const { return __status; }
        QByteArray HttpResponse::body() const { return __body; }
        HttpVersion HttpResponse::httpVersion() const { return __http_version; }
        QMap<QByteArray, QByteArray> HttpResponse::headers() const { return __headers; }
        QByteArray HttpResponse::header(QByteArray const &http_header) const { return __headers.value(http_header); }
    }
}