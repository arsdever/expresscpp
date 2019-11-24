#include "http_response_impl.h"
#include <QDebug>

namespace ad
{
    namespace http
    {
        HttpResponseImpl::HttpResponseImpl(HttpVersion version, ResponseStatus status, QByteArray body)
            : __http_version(version)
            , __status(status)
            , __body(body)
        {
            addHeader("Content-Type", contentTypeVerbose(ContentType::TXT));
            addHeader("Content-Length", QByteArray::number(body.size()));
        }

        HttpResponseImpl::~HttpResponseImpl() {}

        QByteArray HttpResponseImpl::serialize() const
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

        void HttpResponseImpl::deserialize(QByteArray const &data) {}
        void HttpResponseImpl::setStatus(ResponseStatus status) { __status = status; }
        void HttpResponseImpl::setHttpVersion(HttpVersion version) { __http_version = version; }
        void HttpResponseImpl::addHeader(QByteArray const &key, QByteArray const &value) { __headers.insert(key, value); }
        void HttpResponseImpl::setHeaders(QMap<QByteArray, QByteArray> const &headers) { __headers = headers; }
        void HttpResponseImpl::setBody(QByteArray const &body, ContentType type)
        {
            __body = body;
            addHeader("Content-Type", contentTypeVerbose(type));
            addHeader("Content-Length", QByteArray::number(body.size()));
        }
        ResponseStatus HttpResponseImpl::status() const { return __status; }
        QByteArray HttpResponseImpl::body() const { return __body; }
        HttpVersion HttpResponseImpl::httpVersion() const { return __http_version; }
        QMap<QByteArray, QByteArray> HttpResponseImpl::headers() const { return __headers; }
        QByteArray HttpResponseImpl::header(QByteArray const &http_header) const { return __headers.value(http_header); }
    }
}