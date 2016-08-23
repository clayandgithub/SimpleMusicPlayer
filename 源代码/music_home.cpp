#include "music_home.h"
#include<QMessageBox>
#include<cmath>
#include <iostream>

#define PI 3.1415f

music_home::music_home(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);

	resize(600,500);
	//setWindowOpacity(80);
	mMy_func = my_functions();
	mNeedErase = false;
	mState=NO_EVENT;
    mBoard=QPixmap(500,500);
    mBoard.fill(Qt::black);
	mTimer = NULL;
	mMusic = new My_Music();
	initMusic("卡农.mp3",true);

	mBackground = QPixmap("./pictures/background.png");
	mMenuBar = QPixmap("./pictures/menu_bar.png");
	mSymbols.clear();
	for(int i =1;i < 17;++i)
	{
		char n1 = i/10 + '0';
		char n2 = i%10 + '0';
		QString temp_path("./pictures/m");
		temp_path.append(n1);
		temp_path.append(n2);
		temp_path.append(".png");
		mSymbols.push_back(QPixmap(temp_path));
		//string fileName2 = temp_path.toStdString();
		//cout<<fileName2<<std::endl;
	}
	for(int i =0;i<MAX_PARTICLE_NUM;++i)
	{
		mParticle[i] = new Particle(true);
	}
	connect(ui.playBtn,SIGNAL(released()),this,SLOT(playMusic()));
	connect(ui.pauseBtn,SIGNAL(released()),this,SLOT(pauseMusic()));
	connect(ui.changeBtn,SIGNAL(released()),this,SLOT(changeMusic()));
	connect(ui.slider_jump,SIGNAL(valueChanged(int)),this,SLOT(slider_jump_change()));
	//this->setStyleSheet("background-color:#000000");
	//this->setStyleSheet("background-color:url(D:/m/background1.jpg)");
	ui.label_music_name->setStyleSheet("QLabel{color:#000;font-weight:bold;}");//标签字体颜色黑色,粗体
	ui.label_jump_name->setStyleSheet("QLabel{color:#000;font-weight:bold;}");//标签字体颜色黑色,粗体
	ui.label_jump->setStyleSheet("QLabel{color:#000;font-weight:bold;}");//标签字体颜色黑色,粗体
	ui.playBtn->setStyleSheet("QPushButton{background-image: url(./pictures/btn1.png);border:0 px;color:#000;font-weight:bold;}" 
		"QPushButton:hover{background-image: url(./pictures/btn2.png);}" 
		"QPushButton:pressed{background-image: url(./pictures/btn3.png);}");
	ui.pauseBtn->setStyleSheet("QPushButton{background-image: url(./pictures/btn1.png);border:0 px;color:#000;font-weight:bold;}" 
		"QPushButton:hover{background-image: url(./pictures/btn2.png);}" 
		"QPushButton:pressed{background-image: url(./pictures/btn3.png);}");
	ui.changeBtn->setStyleSheet("QPushButton{background-image: url(./pictures/btn1.png);border:0 px;color:#000;font-weight:bold;}" 
		"QPushButton:hover{background-image: url(./pictures/btn2.png);}" 
		"QPushButton:pressed{background-image: url(./pictures/btn3.png);}");
	/*ui.changeEffectBtn->setStyleSheet("QPushButton{background-image: url(D:/m/btn1.png);border:0 px;color:#FFFFFF;}" 
		"QPushButton:hover{background-image: url(D:/m/btn2.png);}" 
		"QPushButton:pressed{background-image: url(D:/m/btn3.png);}");*/
}

music_home::~music_home()
{
	if(mTimer!=NULL)
	{
		mTimer->stop();
		delete mTimer;
		mTimer = NULL;
	}
	if(mMusic!=NULL)
	{
		delete mMusic;
		mMusic = NULL;
	}
	for(int i =0;i<MAX_PARTICLE_NUM;++i)
	{
		if(mParticle[i]!=NULL)
		{
			delete mParticle[i];
			mParticle[i] = NULL;
		}
	}
}

//draw event
void  music_home::paintEvent(QPaintEvent *)
{
	if(mNeedErase)
    {
        mBoard.fill(Qt::black);
        //painter.eraseRect(0,0,500,500);
        mNeedErase = false;
    }

    QPainter painter(&mBoard);
    QPen pen(Qt::red);
	QBrush brush(Qt::blue);
	painter.setPen(pen);
	painter.setBrush(brush);
	//QBrush b(Qt::blue);
	//QRect temp_rect;

	switch(mState)
    {
    case NO_EVENT:
		painter.drawPixmap(0,0,mBackground);
        break;
    case PLAYING:
		painter.drawPixmap(0,0,mBackground);
		//已经计算好的歌曲的绘制(直接绘制激励点序列)
		if(mMusic->mHasPlay)
		{
			if(mMusic->mCurFrame == mMusic->mInspiredPoint.at(mMusic->mCurInspiredIndex))
			{
				//绘制矩形
				//temp_rect = QRect(200,0,100,20 * mMusic->mSEF[mMusic->mCurFrame]);
				//painter.drawRect(temp_rect);
				if( mMusic->mCurInspiredIndex < mMusic->mInspiredPoint.size()-1)
				{
					++mMusic->mCurInspiredIndex;
				}
			}
			//绘制粒子
			for(int i =0;i<MAX_PARTICLE_NUM;++i)
			{
				//pen.setWidth(mParticle[i]->mSize);
				//pen.setColor(QColor(mParticle[i]->mColor[0],mParticle[i]->mColor[1],mParticle[i]->mColor[2]));
				//painter.setPen(pen);
				//painter.drawPoint(mParticle[i]->mPos[0],mParticle[i]->mPos[1]);
				painter.drawPixmap(mParticle[i]->mPos[0],mParticle[i]->mPos[1],mSymbols[i%16].width(),mSymbols[i%16].height(),mSymbols[i%16]);
			}
		}
		//没有计算好的歌曲,需要边计算边绘制
		else
		{
			//std::cout<<"mSEF["<<mMusic->mCurFrame<<"]="<<mMusic->mSEF[mMusic->mCurFrame]<<std::endl;
			//temp_rect = QRect(200,0,100,20 * mMusic->mSEF[mMusic->mCurFrame]);
			//绘制矩形
			//painter.drawRect(temp_rect);
			//绘制粒子
			for(int i =0;i<MAX_PARTICLE_NUM;++i)
			{
				/*pen.setWidth(mParticle[i]->mSize);
				pen.setColor(QColor(mParticle[i]->mColor[0],mParticle[i]->mColor[1],mParticle[i]->mColor[2]));
				painter.setPen(pen);
				painter.drawPoint(mParticle[i]->mPos[0],mParticle[i]->mPos[1]);*/
				painter.drawPixmap(mParticle[i]->mPos[0],mParticle[i]->mPos[1],mSymbols[i%16].width(),mSymbols[i%16].height(),mSymbols[i%16]);
				//painter.drawPixmap(mParticle[i]->mPos[0],mParticle[i]->mPos[1],mSingleSymbol.width(),mSingleSymbol.height(),mSingleSymbol);
				//painter.drawPixmapFragments();
			}
		}
		++mMusic->mCurFrame;
        break;
    case PAUSE:
        break;
	default:
        break;
    }
    QPainter widget_painter(this);
    widget_painter.drawPixmap(0,0,mBoard);
	widget_painter.drawPixmap(500,0,mMenuBar);
}

//mouse event
void music_home::mousePressEvent(QMouseEvent *)
{
}

void music_home::initMusic(string music_name,bool change_music)
{
	if(mTimer!=NULL)
	{
		mTimer->stop();
	}
	if(mMusic->initMusic(music_name,change_music))
	{
		//音乐创建成功
		//QString s = "HELLO.MP3";
		string temp_name = mMy_func.my_split(music_name,'/',-1);
		//cout<<"name = "<<temp_name<<endl;
		ui.label_music_name->setText(tr(temp_name.c_str()));
	}
	else
	{
		//音乐创建失败
		QMessageBox::warning(this,tr("提示"),tr("文件格式错误,请确定文件为mp3文件!"),tr("确定"));
		if(mMusic->initMusic("卡农.mp3",true))
		{
			QMessageBox::warning(this,tr("提示"),tr("无法申请到系统资源,请重新启动软件!"),tr("确定"));
			//exit(1);
		};
	}
}

//button event
void music_home::playMusic()
{
	if(mMusic==NULL || mMusic->isChannelNULL())
	{
		QMessageBox::warning(this,tr("提示"),tr("音乐未能成功创建!"),tr("确定"));
	}
	else
	{
		if(mState != PLAYING)
		{
			if(mTimer==NULL)
			{
				mTimer = new QTimer(this);
				QObject::connect(mTimer,SIGNAL(timeout()),this,SLOT(timerEvent()));
			}
			mMusic->play();
			setState(PLAYING);
			mTimer->start(1000/FPS);
		}
	}
}
void music_home::pauseMusic()
{
	if(mMusic==NULL || mMusic->isChannelNULL())
	{
		QMessageBox::warning(this,tr("提示"),tr("音乐未能成功创建!"),tr("确定"));
	}
	else
	{
		if(mState != PAUSE)
		{
			setState(PAUSE);
			mMusic->pause();
		}
	}
}

void music_home::changeMusic()
{
	//qWarning("test!");
	//cout<<"test"<<endl;
	pauseMusic();
	QString fileName1 = QFileDialog::getOpenFileName(this, tr("选择音乐"),QDir::currentPath(), tr("music files(*.mp3)"));
	string fileName2 = fileName1.toStdString();
	//cout<<fileName2;
	if(fileName2!="")
	{
		initMusic(fileName2,true);
	}
}


void music_home::slider_jump_change()
{
	int value = ui.slider_jump->value();
	ui.label_jump->setText(tr("%1").arg(value));
}

void music_home::setState(int state)
{
	switch(state)
	{
	case NO_EVENT:
		mState = NO_EVENT;
		break;
	case PLAYING:
		mState = PLAYING;
		break;
	case PAUSE:
		mState = PAUSE;
		break;
	default:
		break;
	}
}

//update event
void music_home::timerEvent()
{

	int jump_coe = ui.slider_jump->value();

	switch(mState)
	{
	case NO_EVENT:
		break;
	case PLAYING:
		//到达音乐的最后一帧,则停止并初始化此歌
		if(mMusic!=NULL && mMusic->mCurFrame >= mMusic->mMaxFrameNum)
		{
			mTimer->stop();
			if(!mMusic->mHasPlay)
			{
				mMusic->mHasPlay = true;//已经计算过的标志设为true,接下来重新放此歌时就不用重复计算了
				mMusic->detectPoints();
			}
			initMusic(mMusic->mFileName);
			setState(NO_EVENT);
		}
		
		else if(mMusic!=NULL && !mMusic->mHasPlay)//没有放过的音乐要动态计算频谱
		{
			//std::cout<<"recording..."<<std::endl;
			float temp_res[512];
			mMusic->update();
			mMusic->mSEF[mMusic->mCurFrame] = 0.0f;

			//得到频谱信息
			mMusic->getSpectrum(temp_res);

			//半汉宁窗
			//float temp_hanning_window = 0.5 - 0.5 * cos(2*PI*mCurFrame/2*50);
		
		
			//std::cout<<"temp_canny["<<mCurFrame<<"]="<<temp_canny<<std::endl;
			//float temp_canny = 0.0f;
			//printf("mSpectrum[32]=%f\n",32,temp_res[32]);
			for(int i = 0;i< 512;++i)
			{
				temp_res[i] = temp_res[i] * (0.5 - 0.5 * cos(PI + PI*i/512));//半汉宁窗处理(低通滤波)
				temp_res[i] = log(1.0f + 100 * temp_res[i]) / log(1.0f+100.0f);//非线性压缩
				//canny 算子
				//temp_canny = (temp_res[i]/(1*1)) * exp(1.0f * temp_res[i] * temp_res[i]/(2*1*1));
				//temp_res[i] = temp_res[i] * temp_canny;//微分处理(多点加权差分)
				if(temp_res[i]<0.0f)
				{
					temp_res[i] = 0.0f;//半波整流
				}
				//std::cout<<"mSpectrum["<<i<<"]="<<temp_res[i]<<std::endl;
				mMusic->mSEF[mMusic->mCurFrame] += temp_res[i];
			}
			//计算完成
			mMusic->mSumOfSpectrum += mMusic->mSEF[mMusic->mCurFrame];
			for(int i = 0;i<MAX_PARTICLE_NUM;++i)
			{
				if(mMusic->mSEF[mMusic->mCurFrame] >= 1.2f * mMusic->mSumOfSpectrum / (mMusic->mCurFrame+1))
				{
					if(mParticle[i]->mPos[1]>50)
					{
						mParticle[i]->setSpecialSpeed(0.15 * mMusic->mSEF[mMusic->mCurFrame],- jump_coe * mMusic->mSEF[mMusic->mCurFrame]);
					}
					
				}
				mParticle[i]->tick();
			}
			
			//std::cout<<"mSpectrum[64]="<<temp_res[64]<<std::endl;
			//std::cout<<"in function :mSEF["<<mMusic->mCurFrame<<"]="<<mMusic->mSEF[mMusic->mCurFrame]<<std::endl;
		}
		else if(mMusic!=NULL)//放过的音乐不用动态计算频谱
		{
			for(int i = 0;i<MAX_PARTICLE_NUM;++i)
			{
				if(mMusic->mCurFrame == mMusic->mInspiredPoint.at(mMusic->mCurInspiredIndex))
				{
					if(mParticle[i]->mPos[1]>50)
					{
						mParticle[i]->setSpecialSpeed(0.15 * mMusic->mSEF[mMusic->mCurFrame],- jump_coe * mMusic->mSEF[mMusic->mCurFrame]);
					}
					//mParticle[i]->setSpecialSpeed(16 * mMusic->mSEF[mMusic->mCurFrame],- 32 * mMusic->mSEF[mMusic->mCurFrame]);
				}
				mParticle[i]->tick();
			}
		}
		mNeedErase = true;
		update();	
		break;
	case PAUSE:
		break;
	default:
		break;
	}
}
//Additions:

//qDebug("初始化完毕!temp=%f",temp_d);
//ui.gen_btn->setText(tr("生成曲线"));
//ui.spin_a->setVisible(true);
//mA =ui.spin_a->value();

// QString s = QString::number(temp_d,'f',6);
//ui.perlin_label->setText(s);
//  QImage my_img = QImage(100,100,QImage::Format_RGB32);
//qDebug("初始化完毕!");
//QMessageBox::warning(this,tr("Warning"),tr("点的数量超过上限100!"),QMessageBox::Yes);
////setPixel传入的是坐标值而不是i和jhu
/*if(mTimer!=NULL)
	{
		mTimer->stop();
	}
	mMusic->change();
	ui.label_music_name->setText(tr(mMusic->getMusicName().c_str()));*/