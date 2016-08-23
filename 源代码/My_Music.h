#ifndef MY_MUSIC_H
#define MY_MUSIC_H

#include "fmod.hpp"
#include <string>
#include <vector>
#define FPS 20
//#pragma comment(lib,"fmodex_vc.lib")
using namespace std;
using namespace FMOD;

class My_Music
{
public:
    My_Music(void);
    ~My_Music(void);

	bool initMusic(string file_name,bool change_music = false);  // ��ʼ������
    void pause();  // ��ͣ
    void play();   // ����
    void update(); // ����
	string getMusicName();//�õ�������
	void getSpectrum(float* des_array);//�õ������ֵ�ǰ֡��Ƶ��(512ά����)
	void detectPoints();//������ֵļ�����,ֻ�������������ز�����һ��󷽿ɵ��óɹ�
	bool isChannelNULL();
	bool isPaused();

	string mFileName;//������
	int mMaxFrameNum;//��֡��(��ÿ50msһ֡����)
	int mCurFrame;//��ǰ���ŵ�֡
	float mSEF[10*60*FPS];//��Ӧÿһ֡��Ƶ������ͨ��(�10����)
	bool mHasPlay;//�Ƿ��Ѿ������Ƶ������ͨ���������ڲ��ŵ�ʱ�����ٴμ���Ƶ��������ֱ�ӵ��ü���
	vector<int> mInspiredPoint;//ÿ100ms�����ȡһ��������,��ÿ��֡�����ȡһ��
	int mCurInspiredIndex;//��ǰҪ���ŵļ���������
	float mSumOfSpectrum;
    
private:  // FMOD�ı�Ҫ��Ա
    FMOD::System*   mSystem;
    FMOD::Sound*    mSound;
    FMOD::Channel*  mChannel;
	void ERRCHECK( FMOD_RESULT result );// �����⺯��		
	void release_all();
};
#endif // MY_MUSIC_H