#pragma once

#include <telemetry.h>

#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>
#include <map>

class MyServer : public QWidget {
Q_OBJECT
public:
	MyServer(int port, QWidget* parent = 0);
	~MyServer() override = default;

private slots: 
	virtual void slotNewConnection();
	void slotReadClient();
	void slotSendToClient(QTcpSocket* socket, const QString& message);

private:
	QTcpServer* m_server;
	QTextEdit* m_status;
	quint16 m_nextBlockSize;
	size_t m_numberOfClient;
	std::map<QTcpSocket*, QString> m_clientToNumber;
};
