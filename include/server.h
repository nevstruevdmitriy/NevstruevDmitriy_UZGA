#pragma once;

#include <QWidget>

class QTcpServer;
class QTextEdit;
class QTcpSocket;

class MyServer : public QWidget {
Q_OBJECT
public:
	MyServer(int port, QWidget* parent);
	
private slots: 
	virtual void slotNewConnection();
	void slotReadConnect();
private:
	void sendMessage(QTcpSocket* socket, const QString& message);

	QTcpServer* m_server;
	QTextEdit* m_status;
	quint_16 m_nextBlockSize;
};
