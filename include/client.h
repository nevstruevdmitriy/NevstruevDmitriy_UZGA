#pragma once;

#include <QTextEdit>
#include <QTcpSocket>

class QTextEdit;
class QLineEdit;

class MyClient : public QWidget {
Q_OBJECT
public:
	MyClient(const QString& host, int port, QWidget* parent);

private slots:
	void slotReadyRead();
	void slotError(QAbstractSocket::SocketError);
	void slotSendToServer();
	void slotConnected();

private:
	QTcpSocket* m_socket;
	QTextEdit* m_info;
	QLineEdit* m_input;
	quint16 m_nextBlockSize;
}
