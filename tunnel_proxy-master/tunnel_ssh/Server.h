#pragma once

#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include "QTime"

class ForwardPort: public QObject
{
	Q_OBJECT

private:
	QTcpServer *m_ptcpServer;
	QString host;
	quint16 port;

public:
	ForwardPort(quint16 port);
	void setHost(QString _host) { host = _host; };
	void setPort(quint16 _port) { port = _port; };

public slots:
	void slotNewConnection();


};



