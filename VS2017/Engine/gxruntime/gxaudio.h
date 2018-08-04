
#ifndef GXAUDIO_H
#define GXAUDIO_H

#include "StringType.h"
#include "gxsound.h"

class gxRuntime;
struct FSOUND_SAMPLE;

class gxAudio{
public:
	gxRuntime *runtime;

	gxAudio( gxRuntime *runtime );
	~gxAudio();

	gxChannel *play( FSOUND_SAMPLE *sample );
	gxChannel *play3d( FSOUND_SAMPLE *sample,const float pos[3],const float vel[3] );

	void pause();
	void resume();

private:

	/***** GX INTERFACE *****/
public:
	enum{
		CD_MODE_ONCE=1,CD_MODE_LOOP,CD_MODE_ALL
	};

	gxSound *loadSound( String filename,bool use_3d );
	gxSound *verifySound( gxSound *sound );
	void freeSound( gxSound *sound );

	void setPaused( bool paused );	//master pause
	void setVolume( float volume );	//master volume

	void set3dOptions( float roll,float dopp,float dist );

	void set3dListener( const float pos[3],const float vel[3],const float forward[3],const float up[3] );
};

#endif