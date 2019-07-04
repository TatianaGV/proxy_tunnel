#include "ClientHandler.h"

ClientHandler::ClientHandler(QTcpSocket *socket, const QString & host, quint16 port)
{
	leftSock = socket;
	leftSock->setParent(this);

	connect(leftSock, &QTcpSocket::disconnected, this, &ClientHandler::deleteLater);
	QTextStream cout(stdout);
	cout << "New connection registred" << endl;

	rightSock = new QTcpSocket(this);

	connect(rightSock, &QTcpSocket::connected, this, &ClientHandler::onSockConnected);
	connect(rightSock, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &ClientHandler::slotErrorRight);

	rightSock->connectToHost(host, port);

}

ClientHandler::~ClientHandler()
{
	QTextStream cout(stdout);
	cout << "kill" << endl;
}


void ClientHandler::writeToRight()
{
	rightSock->write(leftSock->readAll());
}


void ClientHandler::writeToLeft()
{
	leftSock->write(rightSock->readAll());
}

void ClientHandler::onSockConnected()
{
	connect(leftSock, &QTcpSocket::readyRead, this, &ClientHandler::writeToRight);
	connect(rightSock, &QTcpSocket::readyRead, this, &ClientHandler::writeToLeft);
	if (leftSock->bytesAvailable())
	{
		emit leftSock->readyRead();
	}
	if (rightSock->bytesAvailable())
	{
		emit rightSock->readyRead();
	}

}

void ClientHandler::onClientRead()
{
	QDataStream in(rightSock);
	in.setVersion(QDataStream::Qt_5_3);

	while (true)
	{
		if (!m_nNextBlockSize)
		{
			if (rightSock->bytesAvailable() < sizeof(quint16))
				break;
		}
		in >> m_nNextBlockSize;
		if (rightSock->bytesAvailable() < m_nNextBlockSize)
			break;

		m_nNextBlockSize = 0;
	}
}


void ClientHandler::slotErrorRight(QAbstractSocket::SocketError err)
{
	QString strError =
		"Error: " + (err == QAbstractSocket::HostNotFoundError ?
			"The host or port has not found." :
			err == QAbstractSocket::RemoteHostClosedError ?
			"The remote host is closed." :
			err == QAbstractSocket::ConnectionRefusedError ?
			"The connection was refused." :
			QString(rightSock->errorString())
			);

	QTextStream cout(stdout);
	cout << strError << "\n";
	leftSock->disconnectFromHost();
	rightSock->disconnectFromHost();
}