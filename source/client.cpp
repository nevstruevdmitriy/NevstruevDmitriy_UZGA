#include <client.h>
#include <telemetry.h>

#include <QTcpSocket>
#include <QDateTime>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

MyClient::MyClient(const QString& host, int port, QWidget* parrent) : QWidget(parrent),
																	  m_nextBlockSize(0) {
	m_socket = new QTcpSocket(this);

	m_socket->connectToHost(host, port);

	connect(m_socket, SIGNAL(connected()), SLOT(slotConnected()));
	connect(m_socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(slotError(QAbstractSocket::SocketError)));

	m_info = new QTextEdit;
	m_input = new QLineEdit;

	connect(m_input, SIGNAL(returnPressed()),
			this, SLOT(slotSendToServerMessage()));

	m_info->setReadOnly(true);

	QPushButton* push = new QPushButton("&Send");
	connect(push, SIGNAL(clicked()), SLOT(slotSendToServerMessage()));

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(new QLabel("<H1>Client</H1>"));
	layout->addWidget(m_info);
	layout->addWidget(m_input);
	layout->addWidget(push);
	setLayout(layout);
}

void MyClient::slotReadyRead() {
	QDataStream in(m_socket);
	in.setVersion(QDataStream::Qt_4_2);

	while (true) {
		if (!m_nextBlockSize) {
			if (m_socket->bytesAvailable() < sizeof(quint16)) {
				break;
			}
			in >> m_nextBlockSize;
		}

		if (m_socket->bytesAvailable() < m_nextBlockSize) {
			m_nextBlockSize = 0;
			break;
		}

		QDateTime time;
		QString message;

		in >> time >> message;

		m_info->append(time.toString() + " " + message);
		m_nextBlockSize = 0;
	}
}

void MyClient::slotError(QAbstractSocket::SocketError error) {
	QString strError = "Error: " + 
			(error == QAbstractSocket::HostNotFoundError ? "The host was not found." :
			error == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed" :
			error == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
			QString(m_socket->errorString()));

	m_info->append(strError);
}

void MyClient::slotSendToServerMessage() {
	slotSendToServerInfo(m_socket, m_input->text());
	m_input->setText("");	
}

void MyClient::slotSendToServerInfo(QTcpSocket* server, const QString& message) {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);

	out << quint16(0) << QDateTime::currentDateTime() << message;
	out << quint16(block.size() - sizeof(quint16));

	server->write(block);
}

void MyClient::slotConnected() {
	Telemetry* telemetry = new Telemetry(m_socket, 1, 2, 0, 100);

	connect(m_socket, SIGNAL(disconnected()),
            telemetry, SLOT(slotStop()));
    connect(telemetry, SIGNAL(signalSend(QTcpSocket*, const QString&)),
            this, SLOT(slotSendToServerInfo(QTcpSocket*, const QString&)));
	
	telemetry->start();
}
