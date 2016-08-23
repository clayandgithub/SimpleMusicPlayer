#ifndef PARTICLE_H
#define PARTICLE_H
//#include <time.h>

//重力加速度(并非一定是9.8,可作调节变量)
#define G 512
//粒子最大生命周期(帧数),-1则代表永不消失
#define MAX_TTL -1
//SPF = 1/FPS
#define SPF 0.05f
//#define MAX_AVG_V 2.0f
//#define DAMPING_SPEED 0.04f

class Particle
{
    //粒子对象
public:
    //构造函数提供两种初始化方式,若不填任何参数,则随机生成初始位置,速度,色彩等属性;另一中则是指定属性
    Particle(bool random = true,int ttl = MAX_TTL,
             int m = 2,int size = 10,int x = 0,int y = 0,
             int speed_x = 0,int speed_y = 0,
             int color_r = 255,int color_g = 0,int color_b = 0);
    ~Particle();
	//设置力,速度,位置等属性
    void setForce(int f_x,int f_y);
    void setSpeed(int v_x,int v_y);
	void setSpecialSpeed(float m_x,int base_y);
    void setPosition(int x, int y);
    //生成随机属性
    void genRandomProperty();
    void setRandomColor(int kind = 0);
    //随机函数
    float myFloatRand(float rand_floor,float rand_ceil);
	int myIntRand(int rand_floor,int rand_ceil);
    //每一帧的运动函数
    void tick(bool speed_limit = false);

    //变量
    int mPos[2];//范围(0-----500,0-----500)
    int mColor[3];//RGB  范围(0-----255)
    int mTTL;//生命周期   -1 means never disappear 0 means need to be delete, greater than 0 means the time to libe (单位:帧数)
    int mSpeed[2];
	float mMulX;
    int mForce[2];
    //float mLastForce[2];//上一帧的力
    int mM;//质量
    int mSize;//大小(参考: 4)
    bool mVisible;//可见性
    //float mDampingSpeed;//阻力相关
private:
    //生成随机属性的种子,属于静态变量
    static long random_seed;
};

#endif // PARTICLE_H
