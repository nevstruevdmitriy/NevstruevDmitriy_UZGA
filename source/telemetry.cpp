#include <telemetry.h>
#include <string>

Telemetry::Telemetry(QTcpSocket* client, int start, int step, 
					 int wait, int delay): 
										  	m_start(start), 
											m_step(step), 
											m_wait(wait),
											m_delay(delay),
											m_stop(false),
											m_client(client) {}

void Telemetry::run() {
	msleep(m_wait);
	int ind = m_start;
	while (!m_stop) {
		emit signalSend(m_client, std::to_string(ind).c_str());
		ind += m_step;
		msleep(m_delay);
	}
}

void Telemetry::slotStop() {
	m_stop = true;
}
