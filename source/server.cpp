#include <server.h>

#include <QHostAddress>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QIODevice>

#include <string>

MyServer::MyServer(int port, QWidget* parent) : QWidget(parent),
												m_nextBlockSize(0),
												m_numberOfClient(0) {
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
	layout->addWidget(new QLabel("<H1>Server</H1><H2>Log:</H2>"));
	layout->addWidget(m_status);
	setLayout(layout);
}

void MyServer::slotNewConnection() {
	QTcpSocket* client = m_server->nextPendingConnection();
	Telemetry* telemetry = new Telemetry(client, 0, 2, 50, 100);	
	
	connect(client, SIGNAL(readyRead()),
			this, SLOT(slotReadClient()));

	connect(client, SIGNAL(disconnected()),
			telemetry, SLOT(slotStop()));
	connect(telemetry, SIGNAL(signalSend(QTcpSocket*, const QString&)),
			this, SLOT(slotSendToClient(QTcpSocket*, const QString&)));

	telemetry->start();

	QString clientName = ("Client" + std::to_string(m_numberOfClient)).c_str();
	m_clientToNumber[client] = clientName;
	++m_numberOfClient;
	
	slotSendToClient(client, "Connected! Your name is \"" + clientName + "\"!");
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

		QDateTime time;
		QString message;

		in >> time >> message;

		QString clientName = m_clientToNumber[client];

		m_status->append(clientName + "[" + time.toString() + "]: \n" + message + "\n");

		m_nextBlockSize = 0;
	}
}

void MyServer::slotSendToClient(QTcpSocket* client, const QString& message) {
	QByteArray block;

	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);
	out << quint16(0) << QDateTime::currentDateTime() << message;

	out.device()->seek(0);

	out << quint16(block.size() - sizeof(quint16));

	client->write(block);
}
