#ifndef __MEDIA_FILE_H__
#define __MEDIA_FILE_H__

#include <media/MediaContainer.h>
#include <media/VideoStream.h>
#include <media/AudioStream.h>
#include <vector>
#include <string>

namespace transcode {

class MediaFile {

    private:
		std::string fileUri;
		transcode::MediaContainer mediaContainer;
		std::vector<transcode::AudioStream> audioStreams;
		std::vector<transcode::VideoStream> videoStreams;

    protected:
		explicit MediaFile():
		fileUri(), mediaContainer("", ""),
		videoStreams(), audioStreams() {}

		void setFileUri(const std::string& fileUri)
		    { this->fileUri = fileUri; }

		void setMediaContainer(const MediaContainer& mediaContainer)
		    { this->mediaContainer = mediaContainer; }

		void setVideoStreams(const std::vector<transcode::VideoStream>& videoStreams)
		    { this->videoStreams = videoStreams; }

		void setAudioStreams(const std::vector<transcode::AudioStream>& audioStreams)
			{ this->audioStreams = audioStreams; }

    public:
        explicit MediaFile(const std::string& uri,
        		const transcode::MediaContainer& mc,
        		const std::vector<transcode::AudioStream>& as,
        		const std::vector<transcode::VideoStream>& vs):
        			fileUri(uri), mediaContainer(mc), audioStreams(as),
        			videoStreams(vs) {}

        virtual ~MediaFile() {};

        std::string getFileUri()
	    	const { return fileUri; }

        transcode::MediaContainer getMediaContainer()
	    	const { return mediaContainer; }

        std::vector<transcode::AudioStream>::size_type getAudioStreamSize()
			const { return audioStreams.size(); }

        transcode::AudioStream getAudioStream(
        		const std::vector<transcode::AudioStream>::size_type i)
			const { return audioStreams[i]; }

        std::vector<transcode::VideoStream>::size_type getVideoStreamSize()
	    	const { return videoStreams.size(); }

        transcode::VideoStream getVideoStream(
        		const std::vector<transcode::VideoStream>::size_type i)
	    	const { return videoStreams[i]; }
};

} /* namespace transcode */

#endif /* __MEDIA_FILE_H__ */
