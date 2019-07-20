# ExpressC++

This is a library similar to ExpressJS framework. It was developed for
`C++`/`Qt` developers to make it easier to develop web applications.

# Usage

First of all you have to have installed [Qt 5.12.2](http://download.qt.io/official_releases/qt/5.12/5.12.2/)*

(*note that the application was developed under Qt v5.12.2, that's why the reference is to this version. It may work with the newer versions as well*)

After that download and build the project. It will require extra sources (general [interfaces](https://github.com/opensalad/interfaces) provided by [opensalad organization](https://github.com/opensalad)).
```
cd ~
git clone https://github.com/opensalad/interfaces
git clone https://github.com/arsdever/expresscpp
cd expresscpp
qmake
make
```

*__If case you download components in different folders, please have a look at the `.pro` file of `expresscpp` component to make sure that given include path is correct__*

## Code example

This is a simple code that uses the library.

``` c++
// example.cpp
#include <QCoreApplication>
#include <server.h>

using namespace ad::http;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    Server express;
    express.GET(QRegularExpression("/*"), [](Server::request_t &req, Server::response_t &res) {
        res.setBody("Hello World");
    });
    express.begin();

    return app.exec();
}
```

## Project example

The project file for example.

``` makefile
TEMPLATE = app
TARGET = application
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -L../expresscpp -lexpresscpp
INCLUDEPATH += <PATH_TO_INTERFACES_PROJECT> <PATH_TO_EXPRESSCPP_PROJECT>
SOURCES += example.cpp
QT += core
```

### Result

After all preparings, build the application using the following command from `example` containing directory
``` bash
qmake
make
```
and run it (don't miss to update `LD_LIBRARY_PATH` or simply copy the `expresscpp.so.*` files into directory)
``` bash
LD_LIBRARY_PATH=<PATH_TO_EXPRESSCPP> ./application
```
or
``` bash
cp <PATH_TO_EXPRESSCPP>/*.so* ./
./application
```
or add post-build event into `expresscpp.pro` file to copy built libraries into this path.

Now open any application to make a `HTTP GET call`. Enter uri `localhost:8008/any/path`. Do call and you will get respond
> Hello World

# Contribution

If you would like to contribute, please feel free for any feedback, code contribution, feature offer, code advice and etc. Also feel free to create an issue.

# 3rd party

As you can guess, the library is based on `Qt` library. In future it's planned to remove all `Qt` dependencies.