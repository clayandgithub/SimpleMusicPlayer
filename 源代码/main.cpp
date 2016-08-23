#include "music_home.h"
#include <QtGui/QApplication>
#include <QTextCodec>
#include <stdio.h>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//支持中文
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());//支持中文
	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//支持中文
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QApplication a(argc, argv);
	music_home w;
	w.show();
	return a.exec();
}
