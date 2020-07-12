#include <server.h>

#include <QHostAddress>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QIODevice>

MyServer::MyServer(int port, QWidget* parent) : QWidget(parent),
												m_nextBlockSize(0) {
	m_server = new QTcpServer(this);
	
	if (!m_server->listen(QHostAddress::Any, port)) {
		QMessageBox::critical(0, "Server Error", "Unable the server: " + m_server->errorString());
		m_server->close();
		return;
	}
	connect(m_server, SIGNAL(newConnection()),
			this, SLOT(slotNewConnection()));

	m_status = new QTextEdit;
	m_status->setReadOnly(true);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(new QLabel("<H1>Server</H1>"));
	layout->addWidget(m_status);
	setLayout(layout);
}

void MyServer::slotNewConnection() {
	QTcpSocket* client = m_server->nextPendingConnection();
	
	connect(client, SIGNAL(disconnected()),
			client, SLOT(deleteLater()));

	connect(client, SIGNAL(readyRead()),
			this, SLOT(slotReadClient()));

	sendToClient(client, "Connected!");
}

void MyServer::slotReadClient() {
	QTcpSocket* client = (QTcpSocket*)sender();
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_2);

	while (true) {
		if (!m_nextBlockSize) {
			if (client->bytesAvailable() < sizeof(quint16)) {
				break;
			}

			in >> m_nextBlockSize;
		}

		if (client->bytesAvailable() < m_nextBlockSize) {
			m_nextBlockSize = 0;
			break;
		}

		QTime time;
		QString message;

		in >> time >> message;

		m_status->append(message);

		m_nextBlockSize = 0;

		sendToClient(client, "ok\n");
	}
}

void MyServer::sendToClient(QTcpSocket* client, const QString& message) {
	QByteArray block;

	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);
	out << quint16(0) << QTime::currentTime() << message;

	out.device()->seek(0);

	out << quint16(block.size() - sizeof(quint16));

	client->write(block);
}
