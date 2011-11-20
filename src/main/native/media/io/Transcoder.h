#ifndef __TRANSCODE_STREAM_H__
#define __TRANSCODE_STREAM_H__

#include <media/MediaStream.h>
#include <media/MediaContainer.h>
#include <media/VideoStream.h>
#include <media/AudioStream.h>
#include <media/io/MediaFile.h>
using namespace std;

namespace transcode {

class Transcoder {

	private:
		MediaFile inputFile;
		MediaFile outputFile;

	public:
		Transcoder(MediaFile in, MediaFile out):
			inputFile(in), outputFile(out) {}

		virtual ~Transcoder() {}

		virtual void get(char& c) = 0;

		virtual void get(char *c, int n) = 0;
};

} /* namespace transcode */

#endif /* __TRANSCODE_STREAM_H__ */
