#include "qt_request.h"
#include <QDataStream>

namespace ad
{
    namespace http
    {
        HttpRequest::~HttpRequest() {}

        QByteArray HttpRequest::serialize() const { return QByteArray(); }

        void HttpRequest::deserialize(QByteArray const &data)
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

        void HttpRequest::setMethod(RequestMethod method) { __method = method; }
        void HttpRequest::setTarget(QByteArray const &target) { __target = target; }
        void HttpRequest::setHttpVersion(HttpVersion version) { __http_version = version; }
        void HttpRequest::addHeader(QByteArray const &key, QByteArray const &value) { __headers.insert(key, value); }
        void HttpRequest::setHeaders(QMap<QByteArray, QByteArray> const &headers) { __headers = headers; }
        void HttpRequest::setBody(QByteArray const &body) { __body = body;}

        RequestMethod HttpRequest::method() const { return __method; }
        QByteArray HttpRequest::target() const { return __target; }
        QByteArray HttpRequest::body() const { return __body; }
        HttpVersion HttpRequest::httpVersion() const { return __http_version; }
        QMap<QByteArray, QByteArray> HttpRequest::headers() const { return __headers; }
        QByteArray HttpRequest::header(QByteArray const &http_header) const { return __headers.value(http_header); }
    }
}