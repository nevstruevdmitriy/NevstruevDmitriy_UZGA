#pragma once

#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>

class MyServer : public QWidget {
Q_OBJECT
public:
	MyServer(int port, QWidget* parent = 0);
	~MyServer() override = default;

private slots: 
	virtual void slotNewConnection();
	void slotReadClient();

private:
	void sendToClient(QTcpSocket* socket, const QString& message);

	QTcpServer* m_server;
	QTextEdit* m_status;
	quint16 m_nextBlockSize;
};
