#include <QThread>
#include <QString>
#include <QTcpSocket>

class Telemetry : public QThread {
Q_OBJECT
public:
	Telemetry(QTcpSocket* client, int start, int step, int wait, int delay);
	void run();
signals:
	void signalSend(QTcpSocket* client, const QString& message);
private slots:
	void slotStop();
private:
	int m_start;
	int m_step;
	int m_wait;
	int m_delay;
	bool m_stop;
	QTcpSocket* m_client;
};
