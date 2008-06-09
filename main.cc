#include <QApplication>
#include <QMessageBox>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	srand(time(0));

	QApplication app(argc, argv);

	MainWindow mainWin;
	mainWin.show();

	return app.exec();
}
