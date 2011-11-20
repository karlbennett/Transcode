#ifndef __AUDIO_STREAM_H__
#define __AUDIO_STREAM_H__
#include <media/MediaStream.h>
#include <string>

namespace transcode {

class AudioStream: public MediaStream {

    private:
		int sampleRate;
		int channels;

    public:
        AudioStream(const std::string mimeType, int sr, int c):
	    MediaStream(mimeType), sampleRate(sr), channels(c) {}

        AudioStream(const AudioStream& audioStream):
	    MediaStream(audioStream),
	    sampleRate(audioStream.sampleRate),
	    channels(audioStream.channels) {}

        int getSampleRate() const { return sampleRate; }
        int getChannels() const { return channels; }
};

} /* namespace transcode */

#endif /* __AUDIO_STREAM_H__ */
