#include <server_factory.h>
#include <singleton.macro>
#include "server_impl.h"

SINGLETON(ServerFactory)

ServerFactory::ServerFactory() {}
ServerFactory::~ServerFactory() {}

Server& ServerFactory::requireNew()
{
    static ServerImpl server;
    return server;
}