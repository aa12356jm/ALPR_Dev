#include "mainWindow.h"
#include <QtWidgets/QApplication>
//#include <QTextCodec>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mainWindow w;
	/*QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));*/
	w.show();
	return a.exec();
}