#pragma once

#include <http_response.h>

#include <QString>
#include <QMap>
#include <QByteArray>

namespace ad
{
    namespace http
    {
        class HttpResponseImpl : public HttpResponse
        {
        public:
            HttpResponseImpl(HttpVersion version, ResponseStatus status, QByteArray body = QByteArray());
            ~HttpResponseImpl() override;

            QByteArray serialize() const override;
            void deserialize(QByteArray const &data) override;

            void setStatus(ResponseStatus status) override;
            void setHttpVersion(HttpVersion version) override;
            void setBody(QByteArray const &body, ContentType type = ContentType::TXT) override;
            void addHeader(QByteArray const &key, QByteArray const &value) override;
            void setHeaders(QMap<QByteArray, QByteArray> const &headers) override;

            ResponseStatus status() const override;
            QByteArray body() const override;
            HttpVersion httpVersion() const override;
            QMap<QByteArray, QByteArray> headers() const override;
            QByteArray header(QByteArray const &http_header) const override;

        private:
            HttpVersion __http_version;
            ResponseStatus __status;
            QMap<QByteArray, QByteArray> __headers;
            QByteArray __body;
        };
    }
}
