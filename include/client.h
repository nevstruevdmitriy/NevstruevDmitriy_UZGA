#pragma once

#include <QTextEdit>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QString>
#include <QLabel>

class MyClient : public QWidget {
Q_OBJECT
public:
   /**
    * @param host - QString, host for connecting
	* @param port - integer, port for connecting
	* @param parent - parent widget. parent widget. If you use it like submodule
	*/
	MyClient(const QString& host, int port, QWidget* parent = 0);
	~MyClient() override = default;

private slots:
	void slotReadyRead();
	void slotError(QAbstractSocket::SocketError);
	void slotSendToServerMessage();
	void slotSendToServerInfo(QTcpSocket*, const QString& message);
	void slotConnected();

private:
	void sendToServer(const QString& message, bool isMessage);
	QByteArray getHash(const QString& message);
	
	QTcpSocket* m_socket;
	QTextEdit* m_info;
	QLineEdit* m_input;
	QLabel* m_label;
	quint16 m_nextBlockSize;
};
