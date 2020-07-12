#include <MyClient.h>

MyClient::Myclient(const QString& host, int port, QWidget* parrent) : QWidget(parrent),
																	  m_nextBlockSize(0) {
	m_socket = new QTcpSocket(this);

	m_socket->connectToHost(host, port);

	connect(m_socket, SIGNAL(connected()), SLOT(slotConnected()));
	connect(m_socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

	connect(m_socket, SIGNAL(error(QABstractSocket::SocketError)),
			this, SLOT(slotError(QAbstractSocket::SocketError)));

	m_info = new QTextEdit;
	M_input = new QLineEdit;

	connect(m_input, SIGNAL(returnPressed()),
			this, SLOT(slotSendToServer()));

	m_info->setReadOnly(true);

	QPushButton* push = new QPushButton("&Send");
	connect(push, SIGNAL(clicked), SLOT(slotSendToServer()));

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
			break;
		}

		QTime time;
		QString message;

		in >> time >> message;

		m_info->append(time.toString() + " " + message);
		m_nextBlockSize = 0;
	}
}

void MyClient::slotError(QAbstractSocket::socket error) {
	QString strError = "Error: " + 
			(error == QAbstractSocket::HostNotFoundError ? "The host was not found." :
			error == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed" :
			error == QAbstractSocket::ConnectionRefusedError ? "The connection was refused." :
			QString(m_socket->errorString()));

	m_info->appedn(strError);
}

void MyClient::slotSendServer() {
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_2);

	out << quint16(0) << QTime::currentTime() << m_input->text();
	out << quint16(arrBlock.size() - sizeof(quint16));

	m_socket->write(block);
	m_input->setText("");
}

void MyClient::slotConnected() {
	m_info->append("Received the connectied() signal");
}
