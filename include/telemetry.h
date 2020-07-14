#include <QThread>
#include <QString>
#include <QTcpSocket>

class Telemetry : public QThread {
Q_OBJECT
public:
   /**
	* @param client - argument will be passed to the signal "signalSend"
	* @param start - start for indexing
	* @param step - step for indexing
	* @param wait - timeout before first message
	* @param delay - timestep for indexing
	*/
	Telemetry(QTcpSocket* client, int start, int step, int wait, int delay);
   /**
	* to generate messages
	*/
	void run();
signals:
   /**
	* trigger, when run() generate message
	* @param client - message recipient
	* @param message - text message
	*/
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
