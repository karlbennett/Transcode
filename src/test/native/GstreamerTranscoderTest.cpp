#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <TestUtils.h>
#include <media/io/gstreamer/GstreamerTranscoder.h>
#include <media/io/gstreamer/GstreamerMediaFile.h>
#include <media/io/exception/MediaFileException.h>
#include <string>


const transcode::GstreamerMediaFile MEDIA_FILE1(VIDEO_ONE);
const transcode::GstreamerMediaFile MEDIA_FILE2(VIDEO_TWO);
const transcode::GstreamerMediaFile MEDIA_FILE3(VIDEO_THREE);

const transcode::MediaFile createMediaFile() {

	using namespace transcode;

	MediaContainer mediaContainer("application/ogg", "An Ogg Theora file.");

	AudioStream audioStream("audio/x-vorbis", 2, 48000);
	vector<AudioStream> audioStreams;
	audioStreams.push_back(audioStream);

	VideoStream videoStream("video/x-theora", 427, 240, 0);
	vector<VideoStream> videoStreams;
	videoStreams.push_back(videoStream);

	return MediaFile("", mediaContainer, audioStreams, videoStreams);
}


BOOST_AUTO_TEST_CASE( test_transcode_media_file )
{
	using namespace transcode;

	GstreamerTranscoder transcoder1(MEDIA_FILE1, createMediaFile());
	GstreamerTranscoder transcoder2(MEDIA_FILE2, createMediaFile());
	GstreamerTranscoder transcoder3(MEDIA_FILE3, createMediaFile());
}

BOOST_AUTO_TEST_CASE( test_transcode_empty_input_media_file )
{
	using namespace transcode;

	vector<AudioStream> audioStream;
	vector<VideoStream> videoStream;

	GstreamerTranscoder transcoder(
			MediaFile("", MediaContainer("",""), audioStream, videoStream),
			createMediaFile());
}

BOOST_AUTO_TEST_CASE( test_transcode_empty_output_media_file )
{
	using namespace transcode;

	vector<AudioStream> audioStream;
	vector<VideoStream> videoStream;

	transcode::GstreamerTranscoder transcoder(MEDIA_FILE1,
			MediaFile("", MediaContainer("",""), audioStream, videoStream));
}
