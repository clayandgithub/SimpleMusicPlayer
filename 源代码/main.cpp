#include "music_home.h"
#include <QtGui/QApplication>
#include <QTextCodec>
#include <stdio.h>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//֧������
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());//֧������
	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//֧������
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QApplication a(argc, argv);
	music_home w;
	w.show();
	return a.exec();
}
