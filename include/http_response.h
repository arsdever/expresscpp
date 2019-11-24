#pragma once

#include <http/response.h>

class QByteArray;
template <typename k, typename v>
class QMap;

namespace ad
{
    namespace http
    {
        class HttpResponse : public Response<QByteArray, QMap<QByteArray, QByteArray>>
        {
        public:
            virtual ~HttpResponse() = default;

            virtual QByteArray serialize() const = 0;
            virtual void deserialize(QByteArray const &data) = 0;

            virtual void setStatus(ResponseStatus status) = 0;
            virtual void setHttpVersion(HttpVersion version) = 0;
            virtual void setBody(QByteArray const &body, ContentType type = ContentType::TXT) = 0;
            virtual void addHeader(QByteArray const &key, QByteArray const &value) = 0;
            virtual void setHeaders(QMap<QByteArray, QByteArray> const &headers) = 0;

            virtual ResponseStatus status() const = 0;
            virtual QByteArray body() const = 0;
            virtual HttpVersion httpVersion() const = 0;
            virtual QMap<QByteArray, QByteArray> headers() const = 0;
            virtual QByteArray header(QByteArray const &http_header) const = 0;
        };
    }
}
