/*
 * GstreamerTranscoder.h
 *
 *  Created on: 7 Nov 2011
 *      Author: karl
 */

#ifndef GSTREAMERTRANSCODER_H_
#define GSTREAMERTRANSCODER_H_

#include <media/io/Transcoder.h>
#include <media/io/MediaFile.h>
#include <gst/gst.h>

namespace transcode {

class GstreamerTranscoder: public Transcoder {

	private:
		int bufferSize;
		int bytesRead;
		char *bytes;

		GstElement *pipeline;
		GstElement *sink;
		GstBuffer *buffer;

	public:
		GstreamerTranscoder(MediaFile in, MediaFile out);
		~GstreamerTranscoder();

		void get(char& c);

		void get(char *c, int n);
};

} /* namespace transcode */

#endif /* GSTREAMERTRANSCODER_H_ */
