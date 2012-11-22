// Copyright Malak Salmo 2012-2013.
// Distributed under the GPLv3.
#include <stdio.h>

struct SUBCHUNK1 {
	int CHUNKSIZE; //16 for PCM
	short AUDIOFMT; //PCM = 1
	short NUMCH;	//Mono = 1, Stereo = 2, etc.
	int SAMPRATE; //8000, 44100, etc.
	int BYTERATE; //== SampleRate * NumChannels * BitsPerSample/8
	short BLKALIGN; //== NumChannels * BitsPerSample/8
	short BITSPERSAMP; //8 bits = 8, 16 bits = 16, etc.
};
struct SUBCHUNK2 {
	int CHUNKSIZE; //== NumSamples * NumChannels * BitsPerSample/8
	char* DATA;		 //The actual sound data.
};

class WAVParser {
	private:
		FILE* wavPtr;
		char changeEndian(char);
		char* chunkID;
		int chunkSize;
		SUBCHUNK1 fmtChunk;
		SUBCHUNK2 dataChunk;
		int iterator = 0;
		
	public:
		WAVParser(char*);
		char* nextSample();
		int getAudioFmt();
		short getNumChannels();
		int getSampleRate();
		int getByteRate();
		short getBlockAlign();
		int getDataSize();
		short getBitsPerSample();
		void reset();
		
};
