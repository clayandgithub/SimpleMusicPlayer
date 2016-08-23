#include "Particle.h"
#include <cmath>
#include <time.h>
#include <random>

long Particle::random_seed = 0;//static variable

Particle::Particle(bool random,int ttl,
             int m,int size,int x,int y,
             int speed_x,int speed_y,
             int color_r,int color_g,int color_b)
{
	if(random)
    {
        genRandomProperty();
    }
	else
    {
        mPos[0] = x;
        mPos[1] = y;

        mColor[0] = color_r;
        mColor[1] = color_g;
        mColor[2] = color_b;

        mTTL = ttl;//��λ:frame

        mSpeed[0] = speed_x;
        mSpeed[1] = speed_y;

        mForce[0] = mForce[1]= 0.0f;//F,Fx,Fy,Fz ps:they will be update during caculating
        mM = m;//��λ:kg
        mSize = size;
        mVisible = true;
		mMulX = 1.0f;
//        mDampingSpeed = 0.01f;
    }
}
Particle::~Particle()
{
}

//������,�ٶ�,λ�õ�����
void Particle::setForce(int f_x,int f_y)
{
	//mLastForce[0] = mForce[0];
    //mLastForce[1] = mForce[1];
	mForce[0] =  f_x;
    mForce[1] =  f_y;
}
void Particle::setSpeed(int v_x,int v_y)
{
	mSpeed[0] =  v_x;
    mSpeed[1] =  v_y;
	//�ٶ���һ����Χ�����
	//mSpeed[0] = myIntRand(mSpeed[0] -50 , mSpeed[0] + 50);
	//mSpeed[0] = (int)(myFloatRand(0.8f,1.0f) * mSpeed[0]);
	//mSpeed[1] = (int)(sin(3.1415f * mPos[0] / 250) * mSpeed[1]);
}

void Particle::setSpecialSpeed(float m_x,int base_y)
{
	mMulX = m_x;
	//mSpeed[0] =  (int)(adjust_var * m_x * myIntRand(32,64));
	if(base_y!=0)
	{
		mSpeed[1] = (int)(sin(3.1415f * mPos[0] / 128) * base_y);
		if(mSpeed[1] > 0)
		{
			mSpeed[1] = -mSpeed[1];
		}
	}
	//�ٶ���һ����Χ�����
	//mSpeed[0] = myIntRand(mSpeed[0] -50 , mSpeed[0] + 50);
	//mSpeed[0] = (int)(myFloatRand(0.8f,1.0f) * mSpeed[0]);
	//mSpeed[1] = (int)(sin(3.1415f * mPos[0] / 250) * mSpeed[1]);
}

void Particle::setPosition(int x,int y)
{
	mPos[0] = x;
    mPos[1] = y;
}
//�����������
void Particle::genRandomProperty()//?????????????????????????????
{
	srand(random_seed++);
	//...�����������
	mPos[0] = myIntRand(0,500);
    mPos[1] = 500;
	//mPos[1] = myIntRand(250,500);
	//printf("Y / myIntRand(200,250) = %d\n",mPos[1]);
	setRandomColor(rand()%6);
	mTTL  = MAX_TTL;
	mSpeed[0] = myIntRand(32,64);
    mSpeed[1] = 0;
    //mM = myIntRand(2,8);//��λ:kg
	mM = 2;//��λ:kg
	mForce[0] = 0.0f;// the force in direction x ,it will be update during caculating
	mForce[1]= mM*G;// the force in direction y ,it will be update during caculating
    mSize = myIntRand(2,8);
    mVisible = true;
}
void Particle::setRandomColor(int kind)
{
	//mColor[1] = float(rand()%30000 + 10000)/100000;
    if(kind==0)
    {
        //black
        mColor[0] = 0;
        mColor[1] = 0;
        mColor[2] = 0;
    }
    else if(kind == 1)
    {
        mColor[0] = 255;
        mColor[1] = 255;
        mColor[2] = 255;
        //white
    }
    else if(kind == 2)
    {
        mColor[0] = 255;
        mColor[1] = 255;
        mColor[2] = 0;
        //yellow
    }
    else if(kind == 3)
    {
        mColor[0] = 220;
        mColor[1] = 0;
        mColor[2] = 40;
        //brown
    }
    else if(kind == 4)
    {
        mColor[0] = 255;
        mColor[1] = 40;
        mColor[2] = 255;
        //purple
    }
    else
    {
        mColor[0] = 0;
        mColor[1] = 0;
        mColor[2] = 255;
        //blue
    }
    //�����仯0-1.00
    float temp_offset = float(rand()%10000)/10000;
    for(int i = 0;i<3;++i)
    {
        mColor[i] -= int(temp_offset * mColor[i]);
    }
}
//float�������
float Particle::myFloatRand(float rand_floor,float rand_ceil)
{
	int rand_level = 8192;//1024*8
	int int_floor = (int)rand_floor;
	int int_ceil = (int)rand_ceil;
	return float(rand()%int((rand_ceil - rand_floor) * rand_level) + rand_floor*rand_level)/rand_level;
	//2.0f-6.0f
    //float(rand()%40000 + 20000)/10000;
}
//Int �������
int Particle::myIntRand(int rand_floor,int rand_ceil)
{
	//rand_ceil < 0 || rand_ceil < rand_floor)
	return (rand() % (rand_ceil-rand_floor)) + rand_floor;
}
//ÿһ֡���˶�����
void Particle::tick(bool speed_limit)//?????????????????????????????
{
	if(mTTL == 0 || (mTTL > 0 && --mTTL==0))
    {
        return;
    }

	//���������ٶ�
    for(int i = 0;i<2;++i)
    {
		if(i == 1 && mPos[1] >= 500 )
		{
			//��ײ������,������Ϊ0
		}
		else
		{
			mSpeed[i] += (int)(((float)mForce[i]/mM) * SPF);//��ʽ�������ǰ���ٶ�,��Vt
		}
    }

    //ÿһ֡���Ӹ�����������Լ��˶�

	//mMulX
	if(mPos[0] + int(mMulX * mSpeed[0] * SPF) > 500)
	{
		//ײ���߽�
		mPos[0] = 500;
		mSpeed[0] = -mSpeed[0];//�ٶȲ�˥��,ֱ�ӷ���
	}
	else if(mPos[0] + int(mMulX * mSpeed[0] * SPF) < 0)
	{
		//ײ���߽�
		mPos[0] = 0;
		mSpeed[0] = -mSpeed[0];//�ٶȲ�˥��,ֱ�ӷ���
	}
	else
	{
		mPos[0] += int(mMulX * mSpeed[0] * SPF);
	}

	if(mPos[1] + int(mSpeed[1] * SPF) > 490)
	{
		//ײ���߽�
		mPos[1] = 490;
		mSpeed[1] = -mSpeed[1]/2;//�ٶ�˥��һ�벢����
	}
	else if(mPos[1] + int(mSpeed[1] * SPF) < 0)
	{
		//ײ���߽�
		mPos[1] = 0;
		mSpeed[1] = -mSpeed[1];//�ٶȲ�˥��,ֱ�ӷ���
	}
	else
	{
		mPos[1] += int(mSpeed[1] * SPF);
	}
    //printf("x = %f,y = %f,z = %f",mPos[0],mPos[1],mPos[2]);
}