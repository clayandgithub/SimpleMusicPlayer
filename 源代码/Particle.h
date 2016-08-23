#ifndef PARTICLE_H
#define PARTICLE_H
//#include <time.h>

//�������ٶ�(����һ����9.8,�������ڱ���)
#define G 512
//���������������(֡��),-1�����������ʧ
#define MAX_TTL -1
//SPF = 1/FPS
#define SPF 0.05f
//#define MAX_AVG_V 2.0f
//#define DAMPING_SPEED 0.04f

class Particle
{
    //���Ӷ���
public:
    //���캯���ṩ���ֳ�ʼ����ʽ,�������κβ���,��������ɳ�ʼλ��,�ٶ�,ɫ�ʵ�����;��һ������ָ������
    Particle(bool random = true,int ttl = MAX_TTL,
             int m = 2,int size = 10,int x = 0,int y = 0,
             int speed_x = 0,int speed_y = 0,
             int color_r = 255,int color_g = 0,int color_b = 0);
    ~Particle();
	//������,�ٶ�,λ�õ�����
    void setForce(int f_x,int f_y);
    void setSpeed(int v_x,int v_y);
	void setSpecialSpeed(float m_x,int base_y);
    void setPosition(int x, int y);
    //�����������
    void genRandomProperty();
    void setRandomColor(int kind = 0);
    //�������
    float myFloatRand(float rand_floor,float rand_ceil);
	int myIntRand(int rand_floor,int rand_ceil);
    //ÿһ֡���˶�����
    void tick(bool speed_limit = false);

    //����
    int mPos[2];//��Χ(0-----500,0-----500)
    int mColor[3];//RGB  ��Χ(0-----255)
    int mTTL;//��������   -1 means never disappear 0 means need to be delete, greater than 0 means the time to libe (��λ:֡��)
    int mSpeed[2];
	float mMulX;
    int mForce[2];
    //float mLastForce[2];//��һ֡����
    int mM;//����
    int mSize;//��С(�ο�: 4)
    bool mVisible;//�ɼ���
    //float mDampingSpeed;//�������
private:
    //����������Ե�����,���ھ�̬����
    static long random_seed;
};

#endif // PARTICLE_H
