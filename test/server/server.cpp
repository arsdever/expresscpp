#include <server.h>
#include <server_factory.h>

#include <QCoreApplication>
#include <QRegularExpression>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	Server &server = ServerFactory::requireNew();
	server.GET(QRegularExpression("/"), [](Server::request_t &req, Server::response_t &res) {
		res.setBody("Ol");
	});
	server.begin(8080);

	return app.exec();
}