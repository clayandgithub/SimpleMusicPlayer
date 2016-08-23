#ifndef MUSIC_HOME_H
#define MUSIC_HOME_H

#pragma comment(lib,"fmodex_vc.lib")

#include "ui_music_home.h"
#include "My_Music.h"
#include "Particle.h"
#include "my_functions.h"
#include <QWidget>
#include <QMouseEvent>
#include <QtGui>
#include <QPainter>
#include <vector>

#define MAX_PARTICLE_NUM 512
//mState Constantant
#define NO_EVENT 0
#define PLAYING 1
#define PAUSE 2

using std::vector;
namespace Ui
{
	class music_home;
}
class music_home : public QWidget
{
	Q_OBJECT

	public:
		music_home(QWidget *parent = 0, Qt::WFlags flags = 0);
		~music_home();
		void paintEvent(QPaintEvent *);
		void mousePressEvent(QMouseEvent *);
		void initMusic(string music_name,bool change_music = false);
		void setState(int state);

		//variables
		my_functions mMy_func;
		bool mNeedErase;
		QPixmap mBoard;
		vector<QPixmap> mSymbols;
		QPixmap mBackground;
		QPixmap mMenuBar;
		int mState;//当前所处状态,根据不同状态进行绘制
		Particle* mParticle[MAX_PARTICLE_NUM];
	private:
		Ui::music_homeClass ui;
		My_Music *mMusic;
		QTimer *mTimer;
	private slots:
		void playMusic();
		void pauseMusic();
		void changeMusic();
		void timerEvent();
		void slider_jump_change();
};
#endif // MUSIC_HOME_H