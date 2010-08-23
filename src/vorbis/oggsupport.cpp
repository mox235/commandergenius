/*
 * oggsupport.c
 *
 *  Created on: 06.01.2009
 *      Author: gerstrong
 */

#include "oggsupport.h"
#include <vector>

#ifdef OGG

short openOGGSound(FILE *fp, SDL_AudioSpec *pspec, stHQSound *psound)
{
	// If Ogg detected, decode it into the stream psound->sound_buffer.
	// It must fit into the Audio_cvt structure, so that it can be converted

	int result;
    OggVorbis_File  oggStream;     // stream handle

    if((result = ov_open_callbacks(fp, &oggStream, NULL, 0, OV_CALLBACKS_DEFAULT)) < 0)
    {
        fclose(fp);
        return 1;
    }
    else
    {
    	long bytes;
    	char array[BUFFER_SIZE];
    	std::vector<char> buffer;
        vorbis_info*    vorbisInfo;    // some formatting data
        vorbis_comment* vorbisComment; // user comments

    	int bitStream;
        vorbisInfo = ov_info(&oggStream, -1);
        vorbisComment = ov_comment(&oggStream, -1);

        pspec->format = AUDIO_S16LSB; // Ogg Audio seems to always use this format

        pspec->channels = vorbisInfo->channels;
        pspec->freq = vorbisInfo->rate;

        psound->sound_len = 0;
        do {
			// Read up to a buffer's worth of decoded sound data
			bytes = ov_read(&oggStream, array, BUFFER_SIZE, 0, 2, 1, &bitStream);
			// Append to end of buffer
			buffer.insert(buffer.end(), array, array + bytes);
        } while (bytes > 0);

        ov_clear(&oggStream);

        psound->sound_len = buffer.size();

        psound->sound_buffer = (Uint8*) malloc( psound->sound_len );
        for(Uint32 i=0; i<psound->sound_len ; i++ )
        {
        	memcpy( &(psound->sound_buffer[i]), &(buffer[i]), 1);
        }

		return 0;
    }
}


bool openOGGStream(FILE *fp, SDL_AudioSpec *pspec, OggVorbis_File  &oggStream)
{
	// If Ogg detected, decode it into the stream psound->sound_buffer.
	// It must fit into the Audio_cvt structure, so that it can be converted

	int result;
    if((result = ov_open_callbacks(fp, &oggStream, NULL, 0, OV_CALLBACKS_DEFAULT)) < 0)
    {
        fclose(fp);
        return false;
    }
    else
    {
        vorbis_info*    vorbisInfo;    // some formatting data
        vorbis_comment* vorbisComment; // user comments

        vorbisInfo = ov_info(&oggStream, -1);
        vorbisComment = ov_comment(&oggStream, -1);

        pspec->format = AUDIO_S16LSB; // Ogg Audio seems to always use this format

        pspec->channels = vorbisInfo->channels;
        pspec->freq = vorbisInfo->rate;

		return true;
    }
}

void readOGGStream( OggVorbis_File  &oggStream, char *buffer, size_t size )
{
	int bitStream;
	unsigned long bytes = 0;
	unsigned long pos = 0;
	unsigned int buf_size=size;

	while( pos<size )
	{
		// Read up to a buffer's worth of decoded sound data
		bytes = ov_read(&oggStream, buffer+pos, buf_size-pos, 0, 2, 1, &bitStream);
		pos += bytes;
	}

}

void cleanupOGG(OggVorbis_File  &oggStream)
{
	ov_clear(&oggStream);
}

#endif
