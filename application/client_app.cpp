#include <client.h>

#include <QApplication>
#include <QString>

#include <stdlib.h>

int main(int argc, char** argv) {
	QString host = "localhost";
	int port = 2323;
	if (argc == 3) {
		host = argv[1];
		port = std::atoi(argv[2]);
	}

	if (argc != 1 && argc != 3) {
		printf("invalid number of arguments\n");
		return 0;
	}
	
	QApplication app(argc, argv);
	MyClient client(host, port);

	client.show();

	return app.exec();
}
