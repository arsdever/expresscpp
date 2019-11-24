#pragma once

#include <http/request.h>

class QByteArray;
template <typename k, typename v>
class QMap;

namespace ad
{
    namespace http
    {
        class HttpRequest : public http::Request<QByteArray, QMap<QByteArray, QByteArray>>
        {
        public:
            virtual ~HttpRequest() = default;

            virtual QByteArray serialize() const = 0;
            virtual void deserialize(QByteArray const &data) = 0;

            virtual void setMethod(RequestMethod method) = 0;
            virtual void setTarget(QByteArray const &target) = 0;
            virtual void setHttpVersion(HttpVersion version) = 0;
            virtual void addHeader(QByteArray const &key, QByteArray const &value) = 0;
            virtual void setHeaders(QMap<QByteArray, QByteArray> const &headers) = 0;
            virtual void setBody(QByteArray const &body) = 0;

            virtual RequestMethod method() const = 0;
            virtual QByteArray target() const = 0;
            virtual QByteArray body() const = 0;
            virtual HttpVersion httpVersion() const = 0;
            virtual QMap<QByteArray, QByteArray> headers() const = 0;
            virtual QByteArray header(QByteArray const &http_header) const = 0;
        };
    }
}
