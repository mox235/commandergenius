/*
 * COGGPlayer.cpp
 *
 *  Created on: 17.02.2011
 *      Author: gerhard
 */

#if defined(OGG) || defined(TREMOR)

#include "COGGPlayer.h"

#include "FindFile.h"
#include "sdl/sound/Sampling.h"
#include "CLogFile.h"

COGGPlayer::COGGPlayer(const std::string& filename, const SDL_AudioSpec& AudioSpec) :
m_filename(filename),
m_AudioSpec(AudioSpec),
m_pcm_size(0),
m_music_pos(0),
m_reading_stream(false),
m_bitStream(0)
{
	m_Audio_cvt.buf = NULL;
}

COGGPlayer::~COGGPlayer() {
	close();
}

#if defined(TREMOR)
int ov_fopen(char *path,OggVorbis_File *vf)
{
	int result;
    FILE *fp = fopen(path, "rb");
	if((result = ov_open(fp, vf, NULL, 0)) < 0)
		fclose(fp);
	return result;
}
#endif

bool COGGPlayer::open()
{
	// If Ogg detected, decode it into the stream psound->sound_buffer.
	// It must fit into the Audio_cvt structure, so that it can be converted

    if(ov_fopen((char*)GetFullFileName(m_filename).c_str(), &m_oggStream) != 0)
        return false;

    vorbis_info*    vorbisInfo;    // some formatting data
    vorbis_comment* vorbisComment; // user comments

    vorbisInfo = ov_info(&m_oggStream, -1);
    vorbisComment = ov_comment(&m_oggStream, -1);

    m_AudioFileSpec.format = AUDIO_S16LSB; // Ogg Audio seems to always use this format

    m_AudioFileSpec.channels = vorbisInfo->channels;
    m_AudioFileSpec.freq = vorbisInfo->rate;

    m_pcm_size = ov_pcm_total(&m_oggStream,-1);
    m_pcm_size *= (vorbisInfo->channels*sizeof(Sint16));
    m_music_pos = 0;

	g_pLogFile->ftextOut("OGG-Player: File \"%s\" was opened successfully!<br>", m_filename.c_str());
	int ret = SDL_BuildAudioCVT(&m_Audio_cvt,
			m_AudioFileSpec.format, m_AudioFileSpec.channels, m_AudioFileSpec.freq,
			m_AudioSpec.format, m_AudioSpec.channels, m_AudioSpec.freq);
	if(ret == -1)
		return false;

	const size_t &length = m_AudioSpec.size;
	m_Audio_cvt.len = (length*m_Audio_cvt.len_mult)/m_Audio_cvt.len_ratio;
	m_Audio_cvt.buf = new Uint8[m_Audio_cvt.len];

    return true;
}

bool COGGPlayer::readOGGStream( OggVorbis_File  &oggStream, char *buffer, const size_t &size, const SDL_AudioSpec &OGGAudioSpec )
{
	long bytes = 0;
	unsigned long pos = 0;
	m_reading_stream = true;

	while( pos<size )
	{
		if(m_pcm_size<=0)
			break;
		// Read up to a buffer's worth of decoded sound data
	#if defined(OGG)
		bytes = ov_read(&m_oggStream, buffer+pos, size-pos, 0, 2, 1, &m_bitStream);
	#elif defined(TREMOR)
		bytes = ov_read(&m_oggStream, buffer+pos, size-pos, &m_bitStream);
	#endif
		pos += bytes;
		m_music_pos += bytes;
		if( bytes <= 0 || m_music_pos >= m_pcm_size )
		{
			memset( buffer+pos, OGGAudioSpec.silence, size-pos );
			pos = size;
			m_bitStream = 0;
			m_music_pos = 0;
			m_reading_stream = false;
			return true;
		}
	}
	m_reading_stream = false;
	return false;
}

bool COGGPlayer::readOGGStreamAndResample( OggVorbis_File  &oggStream, char *buffer, const size_t output_size, const size_t input_size, const SDL_AudioSpec &OGGAudioSpec )
{
	char buf[input_size];

	bool eof = readOGGStream( oggStream, buf, input_size, OGGAudioSpec );

	resample((Uint8*)buffer, (Uint8*)buf, output_size, input_size, OGGAudioSpec.format, OGGAudioSpec.channels);

	return eof;
}

void COGGPlayer::readBuffer(Uint8* buffer, Uint32 length)
{
	if(!m_playing)
		return;

	bool rewind = false;

	// read the ogg stream
	if( m_AudioSpec.freq == 48000 )
	{
		Uint32 insize = (m_Audio_cvt.len*441)/480;
		Uint8 mult = m_AudioFileSpec.channels;

		if(m_AudioFileSpec.format == AUDIO_S16)
			mult <<= 1;

		insize /= mult;
		insize++;
		insize *= mult;

		rewind = readOGGStreamAndResample(m_oggStream, (char*)m_Audio_cvt.buf, m_Audio_cvt.len, insize, m_AudioFileSpec);
	}
	else
	{
		rewind = readOGGStream(m_oggStream, (char*)m_Audio_cvt.buf, m_Audio_cvt.len, m_AudioFileSpec);
	}

	// then convert it into SDL Audio buffer
	// Conversion to SDL Format
	SDL_ConvertAudio(&m_Audio_cvt);

	memcpy(buffer, m_Audio_cvt.buf, length);

	if(rewind)
	{
		close();
		open();
		play(true);
	}
}

void COGGPlayer::close()
{
	if(m_Audio_cvt.buf)
		delete [] m_Audio_cvt.buf;
	m_Audio_cvt.buf = NULL;

	while(m_reading_stream);
	m_music_pos = 0;
	m_pcm_size = 0;
	m_playing = false;
	ov_clear(&m_oggStream);
}

#endif
