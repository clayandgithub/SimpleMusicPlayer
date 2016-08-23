#include "My_Music.h"
#include <cmath>
//#include <cstddef>

My_Music::My_Music(void):mSystem(NULL),mSound(NULL),mChannel(NULL)
{
	mFileName = "";
	mMaxFrameNum = 0;
	mCurFrame = 0;
	memset(mSEF,0.0f,10*60*FPS);
	mHasPlay = false;
	mInspiredPoint.clear();
	mCurInspiredIndex = 0;
	mSumOfSpectrum = 0;
}

My_Music::~My_Music(void)
{
	//�ͷ�����ϵͳ,���ֶ���
	release_all();
}

void My_Music::ERRCHECK( FMOD_RESULT result )
 {
    if (result != FMOD_OK)
    {
        printf("FMOD����! errorcode = [%d]\n",result);
    }
}

/*��ʼ��ĳһ�����������,��׼���������*/
bool My_Music::initMusic(string file_name,bool change_music)
{
	FMOD_RESULT result;

	//���ļ�����ȷ��ֵ
	mFileName = file_name;
	mMaxFrameNum = 0;
	mCurFrame = 0;
	mCurInspiredIndex = 0;
	if(change_music)
	{
		//mSEF[10*60*FPS];//��Ӧÿһ֡��Ƶ������ͨ��(�10����)
		mHasPlay = false;
		mInspiredPoint.clear();
		mSumOfSpectrum = 0.0f;
	}
		
	//�ͷ�����ϵͳ,���ֶ���
	release_all();
	// ����System���󲢳�ʼ��
		
	result = System_Create(&mSystem);
	ERRCHECK(result);

	unsigned version = 0;
	result = mSystem->getVersion(&version);
	ERRCHECK(result);
	if( version < FMOD_VERSION )  // ���汾��Ϣ
	{
		printf("����������ʹ��һ���ɰ汾��FMOD %08x.����汾��Ҫ %08x\n",version,FMOD_VERSION);
		
		//�ͷ�����ϵͳ,���ֶ���
		release_all();
		return false;
	}

	// ��ʼ��ϵͳ
	result = mSystem->init(32,FMOD_INIT_NORMAL,	NULL);
	ERRCHECK(result);
	//System�����ʼ�����
	//result = mSystem->createSound(mFileName.c_str(), FMOD_SOFTWARE, 0, &mSound); 
	// ������������
	result = mSystem->createStream(mFileName.c_str(),FMOD_SOFTWARE | FMOD_LOOP_OFF | FMOD_2D,0,&mSound );
	ERRCHECK(result);

	unsigned int temp_length = 0;//��ʱ�洢getLength���صĳ���
	result = mSound->getLength(&temp_length,FMOD_TIMEUNIT_MS);
	ERRCHECK(result);
	//printf("temp_length=%d\n",temp_length);
	mMaxFrameNum = temp_length/(1000/FPS);
	if(mMaxFrameNum >= 12000)
	{
		printf("�����ļ����Ȳ��ܳ���10����\n");
		//�ͷ�����ϵͳ,���ֶ���
		release_all();
		return false;
	}
	//printf("mMaxFrameNum=%d\n",mMaxFrameNum);
    result = mSystem->playSound( FMOD_CHANNEL_FREE,mSound,false,&mChannel );
    ERRCHECK(result);
	pause();
    return true;
}




bool My_Music::isPaused()
{
	bool result = true;
	if(mChannel!=NULL)
	{
		mChannel->getPaused(&result);
	}
	return result;
}
/**-----------------------------------------------------------------------------
*  ��ͣ
*------------------------------------------------------------------------------
*/
void My_Music::pause()
{
	if(mChannel!=NULL)
	{
		bool paused = false;
		mChannel->getPaused(&paused);
		if( !paused )  // ���paused == false����ʾ��δ��ͣ
		{
			mChannel->setPaused(true);
		}
	}
}
/**-----------------------------------------------------------------------------
*  ����
*------------------------------------------------------------------------------
*/
void My_Music::play()
{
	if(mChannel!=NULL)
	{
		bool paused = true;
		mChannel->getPaused(&paused);
		if( paused )
		{
			mChannel->setPaused(false);
		}
	}
}
/**-----------------------------------------------------------------------------
����
*------------------------------------------------------------------------------
*/
void My_Music::update()
{
    if( mSystem != NULL)
    {
        mSystem->update();
    }
}

string My_Music::getMusicName()
{
	return mFileName;
}

void My_Music::getSpectrum(float* des_array)
{
	int numchannels                   = 0;
    int dummy                         = 0;
	FMOD_SOUND_FORMAT dummyformat     = FMOD_SOUND_FORMAT_NONE;
	FMOD_DSP_RESAMPLER dummyresampler = FMOD_DSP_RESAMPLER_LINEAR;
    int count                         = 0;
    int count2                        = 0;
	FMOD_RESULT result;

	mSystem->getSoftwareFormat(&dummy, &dummyformat, &numchannels, &dummy , &dummyresampler, &dummy);
	for (count = 0; count < numchannels; count++)
	{
        result = mSystem->getSpectrum(des_array, 512, count, FMOD_DSP_FFT_WINDOW_TRIANGLE);
		ERRCHECK(result);
    }
}

void My_Music::detectPoints()
{
	if(!mHasPlay)
	{
		return;
	}

	//�����ֵ
	//float sum1 = 0.0f;
	float mean = 0.0f;
	/*for(int i =0;i<mMaxFrameNum;++i)
	{
		sum1 += mSEF[i];
	}*/
	mean = mSumOfSpectrum / mMaxFrameNum;
	//mean = sum1 / mMaxFrameNum;
	//printf("mean = %.6f\n",mean);
	//���㷽��
	float sum2 = 0.0f;
	float variance = 0.0f;
	for(int i =0;i<mMaxFrameNum;++i)
	{
		if(mSEF[i]!=0.0f)
		{
			sum2 += (mSEF[i]-mean) * (mSEF[i]-mean);
		}
	}
	variance = sqrt(sum2 / mMaxFrameNum);
	//printf("variance = %.6f\n",variance);
	//��������
	float threshold = 1.5f * variance;
	//printf("threshold = %.6f\n",threshold);

	//ÿ����֡ɨ��һ�β��ж���֡�е����ֵ�Ƿ�Ϊ������(��������ֵ)
	for(int i =0;i + 1 < mMaxFrameNum ;i+=2)
	{
		if(mSEF[i] > mSEF[i+1])
		{
			if(mSEF[i] > threshold)
			{
				mInspiredPoint.push_back(i);//��������,���뼤����������
			}
		}
		else
		{
			if(mSEF[i+1] > threshold)
			{
				mInspiredPoint.push_back(i+1);//��������,���뼤����������
			}
		}
	}
	printf("�����ļ���������Ϊ;%d\n",mInspiredPoint.size());
}

void My_Music::release_all()
{
	//��ͣ���ͷſռ�
	pause();
	if( mChannel != NULL )
	{
		mChannel->stop();
		mChannel = NULL;
	}
	if( mSound != NULL)
	{
        mSound->release();
		mSound = NULL;
	}
    if( mSystem != NULL )
    {
        mSystem->close();
        mSystem->release();
		mSystem = NULL;
    }
}
bool My_Music::isChannelNULL()
{
	return (mChannel==NULL);
}