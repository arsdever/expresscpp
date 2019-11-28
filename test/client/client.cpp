#include <http/http.h>

#include <QCoreApplication>
#include <QTcpSocket>
#include <QTimer>

QByteArray constructRequest(ad::http::RequestMethod method, QByteArray const &path)
{
	return QByteArray(ad::http::methodVerbose(method)) + ' ' + path + ' ' + ad::http::httpVersionVerbose(ad::http::HttpVersion::Http1_1) + "\r\n";
}

QByteArray constructResponse(ad::http::ResponseStatus status, QByteArray const &body)
{
	return ad::http::httpVersionVerbose(ad::http::HttpVersion::Http1_1) + ' ' + QString::number((int)status).toLatin1() + ' ' + QByteArray(ad::http::reasonPhraseVerbose(status)) + "\r\n" + body + "\r\n\r\n";
}

int main(int argc, char **argv)
{
	int *test_step = new int(0);
	QCoreApplication app(argc, argv);

	QTcpSocket *socket = new QTcpSocket();
	auto writer = [=]() {
		QByteArray res = "";
		switch (*test_step)
		{
			case 0: res = constructRequest(ad::http::RequestMethod::Get, "/");
		}
		socket->write(res);
	};

	auto reader = [=]() {
		QByteArray res = socket->readAll();
		switch (*test_step)
		{
			case 0: assert(res == constructResponse(ad::http::ResponseStatus::Ok, "OK"));
		}
		++*test_step;
		writer();
	};

	QObject::connect(socket, &QTcpSocket::connected, writer);
	QObject::connect(socket, &QTcpSocket::readyRead, reader);
	socket->connectToHost("localhost", 8080);
	QTimer::singleShot(1000, QCoreApplication::instance(), &QCoreApplication::quit);
	return app.exec();
}