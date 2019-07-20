#pragma once

#include <QString>
#include <QMap>
#include <QByteArray>
#include <http/request.h>

namespace ad
{
    namespace http
    {
        class HttpRequest : public http::Request<QByteArray, QMap>
        {
        public:
            HttpRequest() = default;
            ~HttpRequest() override;

            QByteArray serialize() const override;
            void deserialize(QByteArray const &data) override;

            void setMethod(RequestMethod method) override;
            void setTarget(QByteArray const &target) override;
            void setHttpVersion(HttpVersion version) override;
            void addHeader(QByteArray const &key, QByteArray const &value) override;
            void setHeaders(QMap<QByteArray, QByteArray> const &headers) override;
            void setBody(QByteArray const &body) override;

            RequestMethod method() const override;
            QByteArray target() const override;
            QByteArray body() const override;
            HttpVersion httpVersion() const override;
            QMap<QByteArray, QByteArray> headers() const override;
            QByteArray header(QByteArray const &http_header) const override;

        private:
            RequestMethod __method;
            QByteArray __target;
            HttpVersion __http_version;
            QByteArray __body;
            QMap<QByteArray, QByteArray> __headers;
        };
    }
}
