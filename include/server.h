#pragma once

#include <telemetry.h>

#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>
#include <QMap>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MyServer : public QWidget {
Q_OBJECT
public:
	MyServer(int port, QWidget* parent = 0);
	~MyServer() override = default;

private slots: 
	virtual void slotNewConnection();
	void slotReadClient();
	void slotSendToClient(QTcpSocket* socket, const QString& message);
	void slotError(QAbstractSocket::SocketError);

private:
	QString numberToName(int ind);

	QTcpServer* m_server;
	QTextEdit* m_status;
	quint16 m_nextBlockSize;
	size_t m_numberOfClient;
	QMap<QTcpSocket*, int> m_clientToNumber;
	QHBoxLayout* m_clients;
	QVector<QTextEdit*> m_messages;
	QVector<QTextEdit*> m_telemetries;
};
