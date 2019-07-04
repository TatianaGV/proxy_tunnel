#pragma once
#include <qobject.h>
#include <QTcpSocket>
#include <QDataStream>

class ClientHandler: public QObject
{
	Q_OBJECT
public:
	ClientHandler(QTcpSocket * socket, const QString &host, quint16 port);

	~ClientHandler();

private:
	QTcpSocket *leftSock;
	QTcpSocket *rightSock;
	quint16 m_nNextBlockSize;

private slots:
	void onSockConnected();
	void onClientRead();
	void writeToRight();
	void writeToLeft();
	void slotErrorRight(QAbstractSocket::SocketError);
};
