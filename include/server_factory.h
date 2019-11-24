#pragma once
#ifndef SERVER_FACTORY
#define SERVER_FACTORY

class Server;

class ServerFactory
{
    ServerFactory();
    ~ServerFactory();

    static ServerFactory& global();
public:
    static Server& requireNew();
};
#endif