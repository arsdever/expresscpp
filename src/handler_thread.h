#pragma once
#ifndef HANDLER_TASK
#define HANDLER_TASK

#include <QThread>
#include "server_impl.h"

template <typename k, typename v>
class QMap;
class QString;
class QAbstractSocket;
namespace ad
{
	template <typename s, typename m>
	class Logger;
}

typedef ad::Logger<QString, QMap<QString, QString>> logger_t;

class ServerImpl::HandlerThread : public QThread
{
	Q_OBJECT

public:
	HandlerThread(ServerImpl &parent, QAbstractSocket &socket);
	~HandlerThread();

protected:
	void run() override;

private:
	static uint64_t s__serial_number;
	QString __local_id;
	logger_t &__logger;
	ServerImpl &__parent;
	QAbstractSocket &__socket;
	ServerImpl::request_t *__request;
	ServerImpl::response_t *__response;
	bool __completed;
};
#endif