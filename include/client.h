#pragma once

#include <QTextEdit>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QString>

class MyClient : public QWidget {
Q_OBJECT
public:
	MyClient(const QString& host, int port, QWidget* parent = 0);
	~MyClient() override = default;

private slots:
	void slotReadyRead();
	void slotError(QAbstractSocket::SocketError);
	void slotSendToServerMessage();
	void slotSendToServerInfo(QTcpSocket* server, const QString& message);
	void slotConnected();

private:
	QTcpSocket* m_socket;
	QTextEdit* m_info;
	QLineEdit* m_input;
	quint16 m_nextBlockSize;
};
