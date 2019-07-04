#include <QtCore/QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	quint16 port = atoi(argv[2]);
	ForwardPort server(1234);
	server.setHost(argv[1]);
	server.setPort(port);

	return a.exec();
}
