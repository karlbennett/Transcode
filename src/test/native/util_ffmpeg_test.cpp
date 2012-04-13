#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <util/util_ffmpeg.hpp>
#include <util_test.hpp>

extern "C" {
#include "libavformat/avformat.h"
}

#include <iostream>

AVFormatContext blankFormatContext;
AVCodecContext blankCodecContext;

// The media types of the first frames in the media files.
const AVMediaType FRAME_TYPE_AVI = AVMEDIA_TYPE_VIDEO;
const AVMediaType FRAME_TYPE_MKV = AVMEDIA_TYPE_VIDEO;
const AVMediaType FRAME_TYPE_MP4 = AVMEDIA_TYPE_AUDIO;
const AVMediaType FRAME_TYPE_OGV = AVMEDIA_TYPE_AUDIO;
const AVMediaType FRAME_TYPE_FLV = AVMEDIA_TYPE_AUDIO;

/**
 * Helper function for test packet reads all the way to the end of the
 * media file.
 *
 * @param formatContext - the format context that will have the packets
 *      read from it.
 */
static void testReadPackets(AVFormatContext *formatContext) {

    AVPacket *packet = NULL;

    while (NULL != (packet = transcode::util::readNextPacket(formatContext))) {

        // The packet should not be empty.
        BOOST_REQUIRE( 0 < packet->size);

        av_free_packet(packet);
    }

    // Last packet should be NULL.
    BOOST_REQUIRE( NULL == packet);
}

/**
 * Get the codec from the supplied format context that matches the
 * supplied media type.
 *
 * @param formatContext - the format context to get the codec from.
 * @param mediaType - the media type of the required codec.
 * @return the correct codec or NULL if one is not found.
 */
static AVCodecContext *getCodecContext(const AVFormatContext *formatContext,
        const AVMediaType& mediaType) {

    AVStream *stream = NULL;
    AVCodecContext *codec = NULL;

    for (int i = 0; i < formatContext->nb_streams; i++) {

        stream = formatContext->streams[i];
        codec = stream->codec;

        if (mediaType == codec->codec_type)
            return codec;
    }

    return NULL;
}

struct FormatContextFixture {

    FormatContextFixture() : formatContext(NULL) {}
    FormatContextFixture(AVFormatContext *fc) : formatContext(fc) {}

    virtual ~FormatContextFixture() {
        avformat_close_input(&formatContext);
    }

    AVFormatContext *formatContext;
};

struct AVIFormatContextFixture: public FormatContextFixture {

    AVIFormatContextFixture() :
            FormatContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_AVI)) {
    }
};

struct MKVFormatContextFixture: public FormatContextFixture {

    MKVFormatContextFixture() :
            FormatContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_MKV)) {
    }
};

struct MP4FormatContextFixture: public FormatContextFixture {

    MP4FormatContextFixture() :
            FormatContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_MP4)) {
    }
};

struct OGVFormatContextFixture: public FormatContextFixture {

    OGVFormatContextFixture() :
            FormatContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_OGV)) {
    }
};

struct FLVFormatContextFixture: public FormatContextFixture {

    FLVFormatContextFixture() :
            FormatContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_FLV)) {
    }
};

struct MultiFormatContextFixture {

    MultiFormatContextFixture() :
            aviFormatContext(NULL),
                    mkvFormatContext(NULL),
                    mp4FormatContext(NULL),
                    ogvFormatContext(NULL),
                    flvFormatContext(NULL) {
    }
    MultiFormatContextFixture(AVFormatContext *aviFc,
            AVFormatContext *mkvFc,
            AVFormatContext *mp4Fc,
            AVFormatContext *ogvFc,
            AVFormatContext *flvFc) :
            aviFormatContext(aviFc),
                    mkvFormatContext(mkvFc),
                    mp4FormatContext(mp4Fc),
                    ogvFormatContext(ogvFc),
                    flvFormatContext(flvFc) {
    }

    virtual ~MultiFormatContextFixture() {
        avformat_close_input(&aviFormatContext);
        avformat_close_input(&mkvFormatContext);
        avformat_close_input(&mp4FormatContext);
        avformat_close_input(&ogvFormatContext);
        avformat_close_input(&flvFormatContext);
    }

    AVFormatContext *aviFormatContext;
    AVFormatContext *mkvFormatContext;
    AVFormatContext *mp4FormatContext;
    AVFormatContext *ogvFormatContext;
    AVFormatContext *flvFormatContext;
};

struct MultiInitialisedFormatContextFixture : public MultiFormatContextFixture {

    MultiInitialisedFormatContextFixture() :
        MultiFormatContextFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_AVI),
                transcode::util::retrieveAVFormatContext(VIDEO_MKV),
                transcode::util::retrieveAVFormatContext(VIDEO_MP4),
                transcode::util::retrieveAVFormatContext(VIDEO_OGV),
                transcode::util::retrieveAVFormatContext(VIDEO_FLV)) {
    }

    virtual ~MultiInitialisedFormatContextFixture() {}
};

struct PacketFixture : public FormatContextFixture {

    PacketFixture() : FormatContextFixture(NULL), packet(NULL) {}
    PacketFixture(AVFormatContext *fc) :
        FormatContextFixture(fc),
        packet(transcode::util::readNextPacket(formatContext)) {}
    PacketFixture(AVFormatContext *fc, AVPacket *p) :
                FormatContextFixture(fc), packet(p) {}

    ~PacketFixture() {
        av_free_packet(packet);
    }

    AVPacket *packet;
};

struct AVINullPacketFixture : public PacketFixture {

    AVINullPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_AVI),
                NULL) {}
};

struct AVIPacketFixture : public PacketFixture {

    AVIPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_AVI)) {}
};

struct MKVNullPacketFixture : public PacketFixture {

    MKVNullPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_MKV),
                NULL) {}
};

struct MKVPacketFixture : public PacketFixture {

    MKVPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_MKV)) {}
};

struct MP4NullPacketFixture : public PacketFixture {

    MP4NullPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_MP4),
                NULL) {}
};

struct MP4PacketFixture : public PacketFixture {

    MP4PacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_MP4)) {}
};

struct OGVNullPacketFixture : public PacketFixture {

    OGVNullPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_OGV),
                NULL) {}
};

struct OGVPacketFixture : public PacketFixture {

    OGVPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_OGV)) {}
};

struct FLVNullPacketFixture : public PacketFixture {

    FLVNullPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_FLV),
                NULL) {}
};

struct FLVPacketFixture : public PacketFixture {

    FLVPacketFixture() :
        PacketFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_FLV)) {}
};

struct MultiPacketFixture: public MultiFormatContextFixture {

    MultiPacketFixture() :
            MultiFormatContextFixture(),
                    aviPacket(NULL), mkvPacket(NULL), mp4Packet(NULL),
                    ogvPacket(NULL), flvPacket(NULL)
    {
    }
    MultiPacketFixture(AVFormatContext *aviFc, AVFormatContext *mkvFc,
            AVFormatContext *mp4Fc, AVFormatContext *ogvFc,
            AVFormatContext *flvFc) :
            MultiFormatContextFixture(aviFc, mkvFc, mp4Fc, ogvFc, flvFc),
                    aviPacket(
                            transcode::util::readNextPacket(aviFormatContext)),
                    mkvPacket(
                            transcode::util::readNextPacket(mkvFormatContext)),
                    mp4Packet(
                            transcode::util::readNextPacket(mp4FormatContext)),
                    ogvPacket(
                            transcode::util::readNextPacket(ogvFormatContext)),
                    flvPacket(transcode::util::readNextPacket(flvFormatContext))
    {
    }

    virtual ~MultiPacketFixture() {
        av_free_packet(aviPacket);
        av_free_packet(mkvPacket);
        av_free_packet(mp4Packet);
        av_free_packet(ogvPacket);
        av_free_packet(flvPacket);
    }

    AVPacket *aviPacket;
    AVPacket *mkvPacket;
    AVPacket *mp4Packet;
    AVPacket *ogvPacket;
    AVPacket *flvPacket;
};

struct MultiInitialisedPacketFixture : public MultiPacketFixture {

    MultiInitialisedPacketFixture() : MultiPacketFixture(
            transcode::util::retrieveAVFormatContext(VIDEO_AVI),
            transcode::util::retrieveAVFormatContext(VIDEO_MKV),
            transcode::util::retrieveAVFormatContext(VIDEO_MP4),
            transcode::util::retrieveAVFormatContext(VIDEO_OGV),
            transcode::util::retrieveAVFormatContext(VIDEO_FLV))
            {}
};

struct CodecContextFixture: public FormatContextFixture {

    CodecContextFixture() :
            FormatContextFixture(),
                    videoCodec(NULL), audioCodec(NULL) {
    }
    CodecContextFixture(AVFormatContext *fc) :
            FormatContextFixture(fc),
                    videoCodec(getCodecContext(formatContext,
                            AVMEDIA_TYPE_VIDEO)),
                    audioCodec(getCodecContext(formatContext,
                            AVMEDIA_TYPE_AUDIO)) {
    }
    CodecContextFixture(AVFormatContext *fc, AVCodecContext *vc,
            AVCodecContext *ac) :
            FormatContextFixture(fc),
                    videoCodec(vc), audioCodec(ac) {
    }

    virtual ~CodecContextFixture() {
        avcodec_close(videoCodec);
        av_free(videoCodec);
        avcodec_close(audioCodec);
        av_free(audioCodec);
    }

    AVCodecContext *videoCodec;
    AVCodecContext *audioCodec;
};

struct AVICodecContextFixture: public CodecContextFixture {

    AVICodecContextFixture() :
            CodecContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_AVI)) {
    }
};

struct MKVCodecContextFixture: public CodecContextFixture {

    MKVCodecContextFixture() :
            CodecContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_MKV)) {
    }
};

struct MP4CodecContextFixture: public CodecContextFixture {

    MP4CodecContextFixture() :
            CodecContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_MP4)) {
    }
};

struct OGVCodecContextFixture: public CodecContextFixture {

    OGVCodecContextFixture() :
            CodecContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_OGV)) {
    }
};

struct FLVCodecContextFixture: public CodecContextFixture {

    FLVCodecContextFixture() :
            CodecContextFixture(
                    transcode::util::retrieveAVFormatContext(VIDEO_FLV)) {
    }
};

struct MultiCodecContextFixture: public MultiFormatContextFixture {

    MultiCodecContextFixture() :
            MultiFormatContextFixture(),
                    aviAudioCodec(NULL), aviVideoCodec(NULL),
                    mkvAudioCodec(NULL), mkvVideoCodec(NULL),
                    mp4AudioCodec(NULL), mp4VideoCodec(NULL),
                    ogvAudioCodec(NULL), ogvVideoCodec(NULL),
                    flvAudioCodec(NULL), flvVideoCodec(NULL)
    {
    }
    MultiCodecContextFixture(AVFormatContext *aviFc, AVFormatContext *mkvFc,
            AVFormatContext *mp4Fc, AVFormatContext *ogvFc,
            AVFormatContext *flvFc) :
            MultiFormatContextFixture(aviFc, mkvFc, mp4Fc, ogvFc, flvFc),
            aviAudioCodec(
                    getCodecContext(aviFormatContext,
                            AVMEDIA_TYPE_AUDIO)),
                    aviVideoCodec(
                            getCodecContext(aviFormatContext,
                                    AVMEDIA_TYPE_VIDEO)),
                    mkvAudioCodec(
                            getCodecContext(mkvFormatContext,
                                    AVMEDIA_TYPE_AUDIO)),
                    mkvVideoCodec(
                            getCodecContext(mkvFormatContext,
                                    AVMEDIA_TYPE_VIDEO)),
                    mp4AudioCodec(
                            getCodecContext(mp4FormatContext,
                                    AVMEDIA_TYPE_AUDIO)),
                    mp4VideoCodec(
                            getCodecContext(mp4FormatContext,
                                    AVMEDIA_TYPE_VIDEO)),
                    ogvAudioCodec(
                            getCodecContext(ogvFormatContext,
                                    AVMEDIA_TYPE_AUDIO)),
                    ogvVideoCodec(
                            getCodecContext(ogvFormatContext,
                                    AVMEDIA_TYPE_VIDEO)),
                    flvAudioCodec(
                            getCodecContext(flvFormatContext,
                                    AVMEDIA_TYPE_AUDIO)),
                    flvVideoCodec(
                            getCodecContext(flvFormatContext,
                                    AVMEDIA_TYPE_VIDEO))
    {
    }

    virtual ~MultiCodecContextFixture() {

        avcodec_close(aviAudioCodec);
        avcodec_close(aviVideoCodec);

        avcodec_close(mkvAudioCodec);
        avcodec_close(mkvVideoCodec);

        avcodec_close(mp4AudioCodec);
        avcodec_close(mp4VideoCodec);

        avcodec_close(ogvAudioCodec);
        avcodec_close(ogvVideoCodec);

        avcodec_close(flvAudioCodec);
        avcodec_close(flvVideoCodec);
    }

    AVCodecContext *aviAudioCodec;
    AVCodecContext *aviVideoCodec;

    AVCodecContext *mkvAudioCodec;
    AVCodecContext *mkvVideoCodec;

    AVCodecContext *mp4AudioCodec;
    AVCodecContext *mp4VideoCodec;

    AVCodecContext *ogvAudioCodec;
    AVCodecContext *ogvVideoCodec;

    AVCodecContext *flvAudioCodec;
    AVCodecContext *flvVideoCodec;
};

struct MultiInitialisedCodecContextFixture: public MultiCodecContextFixture {

    MultiInitialisedCodecContextFixture() :
        MultiCodecContextFixture(
                transcode::util::retrieveAVFormatContext(VIDEO_AVI),
                transcode::util::retrieveAVFormatContext(VIDEO_MKV),
                transcode::util::retrieveAVFormatContext(VIDEO_MP4),
                transcode::util::retrieveAVFormatContext(VIDEO_OGV),
                transcode::util::retrieveAVFormatContext(VIDEO_FLV)) {}
};

/**
 * Test to make sure that an FFMPEG error message can be found.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_error_message )
{

	BOOST_REQUIRE( transcode::util::UNKNOWN !=
	        transcode::util::ffmpegErrorMessage(0));
}

/**
 * Test to make sure that the correct message is returned if an
 * FFMPEG error message cannot be found.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_error_message_not_found )
{

    BOOST_REQUIRE( transcode::util::UNKNOWN ==
            transcode::util::ffmpegErrorMessage(1));
}

/**
 * Test to make sure that a format context can be found for all
 * the video files.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_retrieve_format_context, MultiFormatContextFixture )
{

    aviFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    mkvFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    mp4FormatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    ogvFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    flvFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an empty string.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_empty_string )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(""),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an invalid file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_invalid_file )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(INVALID_FILE),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from an empty file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_empty_file )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(EMPTY_FILE),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when a format context is requested
 * from a text file.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_format_context_non_media_file )
{

    BOOST_REQUIRE_THROW( transcode::util::retrieveAVFormatContext(TEXT_FILE),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_subtitles, MultiInitialisedFormatContextFixture )
{
    (void) transcode::util::extractSubtitleDetails(aviFormatContext);
    (void) transcode::util::extractSubtitleDetails(mkvFormatContext);
    (void) transcode::util::extractSubtitleDetails(mp4FormatContext);
    (void) transcode::util::extractSubtitleDetails(ogvFormatContext);
    (void) transcode::util::extractSubtitleDetails(flvFormatContext);
}

/**
 * Test to make sure that an exception is thrown when subtitle metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_subtitles_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::extractSubtitleDetails(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when subtitle
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_subtitles_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::extractSubtitleDetails(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_avi_subtitles, AVIFormatContextFixture )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testAVISubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_mkv_subtitles, MKVFormatContextFixture )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testMKVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_mp4_subtitles, MP4FormatContextFixture )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testMP4Subtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_ogv_subtitles, OGVFormatContextFixture )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testOGVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that subtitle metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_flv_subtitles, FLVFormatContextFixture )
{

    std::vector<transcode::SubtitleMetaData> subtitleMetaData =
            transcode::util::extractSubtitleDetails(formatContext);

    testFLVSubtitles(subtitleMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_audio, MultiInitialisedFormatContextFixture )
{
    (void) transcode::util::extractAudioDetails(aviFormatContext);
    (void) transcode::util::extractAudioDetails(aviFormatContext);
    (void) transcode::util::extractAudioDetails(aviFormatContext);
    (void) transcode::util::extractAudioDetails(aviFormatContext);
    (void) transcode::util::extractAudioDetails(aviFormatContext);
}

/**
 * Test to make sure that an exception is thrown when audio metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_audio_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::extractAudioDetails(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when audio
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_audio_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::extractAudioDetails(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_avi_audio, AVIFormatContextFixture )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testAVIAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_mkv_audio, MKVFormatContextFixture )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testMKVAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_mp4_audio, MP4FormatContextFixture )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testMP4Audio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_ogv_audio, OGVFormatContextFixture )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testOGVAudio(audioMetaData);
}

/**
 * Test to make sure that audio metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_flv_audio, FLVFormatContextFixture )
{

    std::vector<transcode::AudioMetaData> audioMetaData =
            transcode::util::extractAudioDetails(formatContext);

    testFLVAudio(audioMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_video, MultiInitialisedFormatContextFixture )
{
    (void) transcode::util::extractVideoDetails(aviFormatContext);
    (void) transcode::util::extractVideoDetails(mkvFormatContext);
    (void) transcode::util::extractVideoDetails(mp4FormatContext);
    (void) transcode::util::extractVideoDetails(ogvFormatContext);
    (void) transcode::util::extractVideoDetails(flvFormatContext);
}

/**
 * Test to make sure that an exception is thrown when video metadata
 * is requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_video_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::extractVideoDetails(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty collection is returned when video
 * metadata is requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_video_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::extractVideoDetails(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_avi_video, AVIFormatContextFixture )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testAVIVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_mkv_video, MKVFormatContextFixture )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testMKVVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_mp4_video, MP4FormatContextFixture )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testMP4Video(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_ogv_video, OGVFormatContextFixture )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testOGVVideo(videoMetaData);
}

/**
 * Test to make sure that video metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_extract_flv_video, FLVFormatContextFixture )
{

    std::vector<transcode::VideoMetaData> videoMetaData =
            transcode::util::extractVideoDetails(formatContext);

    testFLVVideo(videoMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_build_container, MultiInitialisedFormatContextFixture  )
{
    (void) transcode::util::buildContainerDetail(aviFormatContext);
    (void) transcode::util::buildContainerDetail(mkvFormatContext);
    (void) transcode::util::buildContainerDetail(mp4FormatContext);
    (void) transcode::util::buildContainerDetail(ogvFormatContext);
    (void) transcode::util::buildContainerDetail(flvFormatContext);
}

/**
 * Test to make sure that an exception is thrown when container metadata
 * are requested on NULL.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_container_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::buildContainerDetail(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an exception is thrown when container metadata
 * are requested on an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_retrieve_container_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::buildContainerDetail(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_build_avi_container, AVIFormatContextFixture )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testAVIContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_build_mkv_container, MKVFormatContextFixture )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testMKVContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an MP4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_build_mp4_container, MP4FormatContextFixture )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testMP4Container(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_build_ogv_container, OGVFormatContextFixture )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testOGVContainer(containerMetaData);
}

/**
 * Test to make sure that container metadata can be extracted from a format
 * context built from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_build_flv_container, FLVFormatContextFixture )
{

    transcode::ContainerMetaData containerMetaData =
            transcode::util::buildContainerDetail(formatContext);

    testFLVContainer(containerMetaData);
}

/**
 * Test to make sure that a packet can be read from all the different container
 * types.
 */
BOOST_FIXTURE_TEST_CASE( test_read_next_packet, MultiInitialisedFormatContextFixture )
{

    av_free_packet(transcode::util::readNextPacket(aviFormatContext));
    av_free_packet(transcode::util::readNextPacket(mkvFormatContext));
    av_free_packet(transcode::util::readNextPacket(mp4FormatContext));
    av_free_packet(transcode::util::readNextPacket(ogvFormatContext));
    av_free_packet(transcode::util::readNextPacket(flvFormatContext));
}

BOOST_AUTO_TEST_CASE( test_read_next_packet_from_null )
{

    BOOST_REQUIRE_THROW( transcode::util::readNextPacket(NULL),
            transcode::util::FFMPEGException);
}

BOOST_AUTO_TEST_CASE( test_read_next_packet_from_blank_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::readNextPacket(&blankFormatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_avi_packet, AVINullPacketFixture )
{

    packet = transcode::util::readNextPacket(formatContext);

    BOOST_REQUIRE( NULL != packet );
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_avi_packets, AVIFormatContextFixture )
{

    testReadPackets(formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_mkv_packet, MKVNullPacketFixture )
{

    packet = transcode::util::readNextPacket(formatContext);

    BOOST_REQUIRE( NULL != packet);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_mkv_packets, MKVFormatContextFixture )
{

    testReadPackets(formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_mp4_packet, MP4NullPacketFixture )
{

    packet = transcode::util::readNextPacket(formatContext);

    BOOST_REQUIRE( NULL != packet);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_mp4_packets, MP4FormatContextFixture )
{

    testReadPackets(formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_ogv_packet, OGVNullPacketFixture )
{

    packet = transcode::util::readNextPacket(formatContext);

    BOOST_REQUIRE( NULL != packet);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_ogv_packets, OGVFormatContextFixture )
{

    testReadPackets(formatContext);
}

/**
 * Test to make sure that a packet can be read from a format context built
 * from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_flv_packet, FLVNullPacketFixture )
{

    packet = transcode::util::readNextPacket(formatContext);

    BOOST_REQUIRE( NULL != packet);
}

/**
 * Test to make sure that all the packets can be read from a format context
 * built from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_read_flv_packets, FLVFormatContextFixture )
{

    testReadPackets(formatContext);
}

/**
 * Test to make sure that a packets type can be found when read from all the
 * different container types.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_find_packet_type, MultiInitialisedPacketFixture )
{

    (void) transcode::util::findPacketType(aviPacket, aviFormatContext);
    (void) transcode::util::findPacketType(mkvPacket, mkvFormatContext);
    (void) transcode::util::findPacketType(mp4Packet, mp4FormatContext);
    (void) transcode::util::findPacketType(ogvPacket, ogvFormatContext);
    (void) transcode::util::findPacketType(flvPacket, flvFormatContext);
}

/**
 * Test to make sure the right packet type is found for avi packets.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_find_avi_packet_type, AVIPacketFixture )
{

    BOOST_REQUIRE_EQUAL( FRAME_TYPE_AVI,
            transcode::util::findPacketType(packet, formatContext));
}

/**
 * Test to make sure the right packet type is found for mkv packets.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_find_mkv_packet_type, MKVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( FRAME_TYPE_MKV,
            transcode::util::findPacketType(packet, formatContext));
}

/**
 * Test to make sure the right packet type is found for mp4 packets.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_find_mp4_packet_type, MP4PacketFixture )
{

    BOOST_REQUIRE_EQUAL( FRAME_TYPE_MP4,
            transcode::util::findPacketType(packet, formatContext));
}

/**
 * Test to make sure the right packet type is found for ogv packets.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_find_ogv_packet_type, OGVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( FRAME_TYPE_OGV,
            transcode::util::findPacketType(packet, formatContext));
}

/**
 * Test to make sure the right packet type is found for flv packets.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_find_flv_packet_type, FLVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( FRAME_TYPE_FLV,
            transcode::util::findPacketType(packet, formatContext));
}

/**
 * Test to make sure that the codec contexts can be opened for the different media files.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_open_codecs, MultiInitialisedCodecContextFixture )
{

    (void) transcode::util::openCodecContext(aviAudioCodec);
    (void) transcode::util::openCodecContext(aviVideoCodec);
    (void) transcode::util::openCodecContext(mkvAudioCodec);
    (void) transcode::util::openCodecContext(mkvVideoCodec);
    (void) transcode::util::openCodecContext(mp4AudioCodec);
    (void) transcode::util::openCodecContext(mp4VideoCodec);
    (void) transcode::util::openCodecContext(ogvAudioCodec);
    (void) transcode::util::openCodecContext(ogvVideoCodec);
    (void) transcode::util::openCodecContext(flvAudioCodec);
    (void) transcode::util::openCodecContext(flvVideoCodec);
}

BOOST_AUTO_TEST_CASE( test_ffmpeg_open_null_codec )
{

    BOOST_REQUIRE_THROW( transcode::util::openCodecContext(NULL),
            transcode::util::FFMPEGException);
}

BOOST_AUTO_TEST_CASE( test_ffmpeg_open_empty_codec )
{

    BOOST_REQUIRE_THROW( transcode::util::openCodecContext(&blankCodecContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that populated format contexts can have their codecs
 * closed.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_close_codecs, MultiInitialisedFormatContextFixture )
{

    (void) transcode::util::closeCodecs(aviFormatContext);
    (void) transcode::util::closeCodecs(mkvFormatContext);
    (void) transcode::util::closeCodecs(mp4FormatContext);
    (void) transcode::util::closeCodecs(ogvFormatContext);
    (void) transcode::util::closeCodecs(flvFormatContext);
}

/**
 * Test to make sure that trying to close NULL throws an exception.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs_with_null )
{

    BOOST_REQUIRE_THROW( transcode::util::closeCodecs(NULL),
                transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an empty format contexts can have its codecs
 * closed.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_close_codecs_with_empty_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::closeCodecs(&blankFormatContext),
            transcode::util::FFMPEGException);
}
