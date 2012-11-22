// Copyright Malak Salmo 2012-2013.
// Distributed under the GPLv3.
#include "WAVParser.h"

WAVParser::WavParser(char* fileName){
	char* wavHeader = new char[12];
	char* fmtHeader = new char[24];
	char* dataHeader;
	
	fmtChunk.CHUNKSIZE = 0;
	fmtChunk.AUDIOFMT = 0;
	fmtChunk.NUMCH = 0;
	fmtChunk.SAMPRATE = 0;
	fmtChunk.BYTERATE = 0;
	fmtChunk.BLKALIGN = 0;
	fmtChunk.BITSPERSAMP = 0;
	
	dataChunk.CHUNKSIZE = 0;
	iterator = 0;
	
	chunkID = new char[4];
	
	wavPtr = fopen(fileName, "r");
	if (wavPtr == NULL) fprintf(stderr,"Error opening wav file.\n");
	else {
		
		if( fgets(wavHeader, 12, wavFile) != NULL ) {
		 //Read in WAV header.
			
			//Extract ID and chunk size info.
			for (int i = 0 ; i < 4 ; i++)
				chunkID[i] = wavHeader[i];
			
			for (int i = 7 ; i <= 4 ; i--)
				chunkSize = (chunksize << 8) + wavHeader[i];
		}
		
		if( fgets(fmtHeader, 24, wavFile) != NULL ) {
		//Read in subchunk 1
			//Chunk Size
			for (int i = 7 ; i >= 4 ; i--)
				fmtChunk.CHUNKSIZE = (fmtChunk.CHUNKSIZE << 8) + fmtHeader[i];
			//Audio Format
			for (int i = 9 ; i >= 8 ; i--)
				fmtChunk.AUDIOFMT = (fmtChunk.AUDIOFMT << 8) + fmtHeader[i];
			//Number of Channels
			for (int i = 11 ; i >= 10 ; i--)
				fmtChunk.NUMCH = (fmtChunk.NUMCH << 8) + fmtHeader[i];
			//Sample Rate
			for (int i = 15; i >= 12 ; i--)
				fmtChunk.SAMPRATE = (fmtChunk.SAMPRATE << 8) + fmtHeader[i];
			//Byte Rate
			for (int i = 19; i >= 16 ; i--)
				fmtChunk.BYTERATE = (fmtChunk.BYTERATE << 8) + fmtHeader[i];
			//Block Align
			for (int i = 21; i >= 20 ; i--)
				fmtChunk.BLKALIGN = (fmtChunk.BLKALIGN << 8) + fmtHeader[i];
			//Bits per sample
			for ( int i = 23 ; i >= 22; i--)
				fmtChunk.BITSPERSAMP = (fmtChunk.BITSPERSAMP << 8) + fmtHeader[i];
		}
		if( fgets(dataHeader, 8, wavFile) != NULL ) { 
		//Read in subchunk 2 header for data size
			for( int i = 7; i >= 4; i--)
				dataChunk.CHUNKSIZE = (dataChunk.CHUNKSIZE << 8) + dataHeader[i];
			dataChunk.DATA = new char[dataChunk.CHUNKSIZE];
			fgets(dataChunk.DATA, dataChunk.CHUNKSIZE, wavFile);
		}
		

	}
}
/* Function: changeEndian()
 * Arguments: A byte, represented as a char.
 * Returns: char
*/
char WAVParser::changeEndian(char c) { return (c >> 4); }

//Get the next sample from the data chunk.
char* WAVParser::nextSample(){
	int returnSize = (fmtChunk.BITSPERSAMP/8)*(fmtChunk.NUMCH);
	char* toReturn = new char[returnSize];

	for(int i = 0; i < returnSize; i++)
		toReturn[i] = dataChunk.DATA[iterator+i];
		iterator+=returnSize;

	return toReturn;
}

/*Get the audio format of the wav file from the first subchunk.
 *
*/
int WAVParser::getAudioFmt(){
	return (int)(fmtChunk.AUDIOFMT);
}

/* 
 *
*/
short WAVParser::getNumChannels(){
	return fmtChunk.NUMCH;
}

int WAVParser::getSampleRate(){
	return fmtChunk.SAMPRATE;
}

int WAVParser::getByteRate(){
	return fmtChunk.BYTERATE;
}

short WAVParser::getBlockAlign(){
	return fmtChunk.BLKALIGN;
}

int WAVParser::getDataSize(){
	return dataChunk.CHUNKSIZE;
}

short WAVParser::getBitsPerSample(){
	return fmtChunk.BITSPERSAMP;
} 

int reset(){ iterator = 0; }
