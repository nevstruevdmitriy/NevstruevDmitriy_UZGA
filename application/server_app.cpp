#include <server.h>

#include <QApplication>
#include <thread>
#include <chrono>
#include <string>
#include <functional>

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	MyServer server(2323);

	server.show();

	return app.exec();
}
