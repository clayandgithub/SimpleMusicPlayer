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

	bool initMusic(string file_name,bool change_music = false);  // 初始化音乐
    void pause();  // 暂停
    void play();   // 播放
    void update(); // 更新
	string getMusicName();//得到音乐名
	void getSpectrum(float* des_array);//得到此音乐当前帧的频谱(512维向量)
	void detectPoints();//检测音乐的激励点,只有在音乐完整地播放完一遍后方可调用成功
	bool isChannelNULL();
	bool isPaused();

	string mFileName;//音乐名
	int mMaxFrameNum;//总帧数(按每50ms一帧计算)
	int mCurFrame;//当前播放的帧
	float mSEF[10*60*FPS];//对应每一帧的频谱能量通量(最长10分钟)
	bool mHasPlay;//是否已经计算过频谱能量通量，是则在播放的时候不用再次计算频谱能量，直接调用即可
	vector<int> mInspiredPoint;//每100ms最多提取一个激励点,即每两帧最多提取一个
	int mCurInspiredIndex;//当前要播放的激励点索引
	float mSumOfSpectrum;
    
private:  // FMOD的必要成员
    FMOD::System*   mSystem;
    FMOD::Sound*    mSound;
    FMOD::Channel*  mChannel;
	void ERRCHECK( FMOD_RESULT result );// 错误检测函数		
	void release_all();
};
#endif // MY_MUSIC_H