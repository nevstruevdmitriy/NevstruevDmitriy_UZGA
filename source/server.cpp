#include <server.h>

#include <QHostAddress>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QIODevice>

#include <string>

MyServer::MyServer(int port, QWidget* parent) : QWidget(parent),
												m_nextBlockSize(0),
												m_numberOfClient(0) {
	setMinimumSize(1000, 500);
	m_server = new QTcpServer(this);

	if (!m_server->listen(QHostAddress::Any, port)) {
		QMessageBox::critical(0, "Server Error", "Unable the server: " + m_server->errorString());
		m_server->close();
		return;
	}
	connect(m_server, SIGNAL(newConnection()),
			this, SLOT(slotNewConnection()));

	QVBoxLayout* layout = new QVBoxLayout;
	m_clients = new QHBoxLayout;
	layout->addWidget(new QLabel("<H1>Server</H1><H2>Clients:</H2>"));
	layout->addLayout(m_clients);
	setLayout(layout);
}

void MyServer::slotNewConnection() {
	QTcpSocket* client = m_server->nextPendingConnection();
	Telemetry* telemetry = new Telemetry(client, 0, 2, 0, 50);	
	
	connect(client, SIGNAL(readyRead()),
			this, SLOT(slotReadClient()));

	connect(client, SIGNAL(disconnected()),
			telemetry, SLOT(slotStop()));
	connect(telemetry, SIGNAL(signalSend(QTcpSocket*, const QString&)),
			this, SLOT(slotSendToClient(QTcpSocket*, const QString&)));

	connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));

	telemetry->start();

	QString clientName = numberToName(m_numberOfClient);
	m_clientToNumber[client] = m_numberOfClient;
	++m_numberOfClient;
	
	slotSendToClient(client, clientName);

	QVBoxLayout* new_client = new QVBoxLayout;
	QTextEdit* new_messages = new QTextEdit;
	QTextEdit* new_telemetries = new QTextEdit;

	new_messages->setReadOnly(true);
	new_telemetries->setReadOnly(true);

	new_client->addWidget(new QLabel("<H3>" + clientName + "</H3>"));
	new_client->addWidget(new QLabel("messages"));
	new_client->addWidget(new_messages);
	new_client->addWidget(new QLabel("telemetries"));
	new_client->addWidget(new_telemetries);

	m_clients->addLayout(new_client);
	
	m_messages.push_back(new_messages);
	m_telemetries.push_back(new_telemetries);
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
		bool isMessage;

		in >> isMessage >> message;

		if (isMessage) {
			in >> time;
		}

		int clientNumber = m_clientToNumber[client];
		QString clientName = numberToName(clientNumber);

		if (isMessage) {
			QString formatMessage = "[" + time.toString() + "]: \n" + message;
			m_messages[clientNumber]->append(formatMessage);
		} else {
			QString formatMessage = message;
			m_telemetries[clientNumber]->append(formatMessage);
		}

		m_nextBlockSize = 0;
	}
}

void MyServer::slotSendToClient(QTcpSocket* client, const QString& message) {
	QByteArray block;

	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);
	out << quint16(0) << message;

	out.device()->seek(0);

	out << quint16(block.size() - sizeof(quint16));

	client->write(block);
}

void MyServer::slotError(QAbstractSocket::SocketError error) {
	QTcpSocket* client = (QTcpSocket*)sender();
	QString strError = "Error: " +
			(error == QAbstractSocket::HostNotFoundError ? "The host was not found." :
			error == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed" :
			error == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
			QString(client->errorString()));

	m_messages[m_clientToNumber[client]]->append(strError);
}

QString MyServer::numberToName(int ind) {
	return ("Client" + std::to_string(ind)).c_str();
}
