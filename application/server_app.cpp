#include <server.h>

#include <QApplication>

// number of arguments must be two:
// second argument is port
int main(int argc, char** argv) {
	QApplication app(argc, argv);

	int port = 2323;
	if (argc == 2) {
		port = std::atoi(argv[1]);
	}

	if (argc != 1 && argc != 2) {
		printf("invalid number of arguments\n");
		return 0;
	}

	MyServer server(port);

	server.show();

	return app.exec();
}
