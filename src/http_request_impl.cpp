#include "http_request_impl.h"
#include <QDataStream>

namespace ad
{
    namespace http
    {
        HttpRequestImpl::~HttpRequestImpl() {}

        QByteArray HttpRequestImpl::serialize() const { return QByteArray(); }

        void HttpRequestImpl::deserialize(QByteArray const &data)
        {
            QByteArray var_data = data;
            var_data.replace("\r\n", "\n");
            QList<QByteArray> lines = var_data.split('\n');
            QList<QByteArray> first_line = lines[0].split(' ');
            QByteArray const &method = first_line[0];
            QByteArray const &target = first_line[1];
            QByteArray const &version = first_line[2];

            if(method == "GET")
                setMethod(ad::http::RequestMethod::Get);
            else if(method == "POST")
                setMethod(ad::http::RequestMethod::Post);
            else if(method == "PUT")
                setMethod(ad::http::RequestMethod::Put);
            else if(method == "DELETE")
                setMethod(ad::http::RequestMethod::Delete);

            setTarget(target);

            if(version == "HTTP/1.1")
                setHttpVersion(ad::http::HttpVersion::Http1_1);
            else if(version == "HTTP")
                setHttpVersion(ad::http::HttpVersion::Http);
            else
                setHttpVersion(ad::http::HttpVersion::Unsupported);

            int i = 1;
            for(; i < lines.size() && !lines[i].isEmpty(); ++i)
            {
                QList<QByteArray> header_line = lines[i].split(':');
                QByteArray header = header_line[0];
                QByteArray value = header_line[1];
                int spaces = 0;
                while(value[spaces] == ' ')
                    ++spaces;

                if(spaces)
                    value.remove(0, spaces);

                addHeader(header, value);
            }

            setBody(lines.mid(i).join("\r\n"));
        }

        void HttpRequestImpl::setMethod(RequestMethod method) { __method = method; }
        void HttpRequestImpl::setTarget(QByteArray const &target) { __target = target; }
        void HttpRequestImpl::setHttpVersion(HttpVersion version) { __http_version = version; }
        void HttpRequestImpl::addHeader(QByteArray const &key, QByteArray const &value) { __headers.insert(key, value); }
        void HttpRequestImpl::setHeaders(QMap<QByteArray, QByteArray> const &headers) { __headers = headers; }
        void HttpRequestImpl::setBody(QByteArray const &body) { __body = body;}

        RequestMethod HttpRequestImpl::method() const { return __method; }
        QByteArray HttpRequestImpl::target() const { return __target; }
        QByteArray HttpRequestImpl::body() const { return __body; }
        HttpVersion HttpRequestImpl::httpVersion() const { return __http_version; }
        QMap<QByteArray, QByteArray> HttpRequestImpl::headers() const { return __headers; }
        QByteArray HttpRequestImpl::header(QByteArray const &http_header) const { return __headers.value(http_header); }
    }
}