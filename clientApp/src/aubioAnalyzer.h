#ifndef _AUBIO_SOUND_PROCESSOR
#define _AUBIO_SOUND_PROCESSOR

#include "aubio.h"
#include "ofMain.h"
#include "ofAddons.h"

class aubioAnalyzer{
	
	public:
		
		void setup();
		void processAudio(float * buffer, int bufferSize);		
		
		~aubioAnalyzer();
	
		float pitch;
		float amplitude;
		
		// ------------------------------------------------------------------
		// internal aubio variables

		uint win_s;							/* window size */
		uint hop_s;							/* hop size */
		uint samplerate;						/* samplerate */
		uint channels;						/* number of channel */
		
		aubio_pitchdetection_mode mode;
		aubio_pitchdetection_type type;

		fvec_t * in;
		aubio_pitchdetection_t * pitch_output ;
	
		// ------------------------------------------------------------------

};

#endif	

