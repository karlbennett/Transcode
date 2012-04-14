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
AVPacket blankPacket;

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
 * Get the stream from the supplied format context that matches the
 * supplied media type.
 *
 * @param formatContext - the format context to get the stream from.
 * @param mediaType - the media type of the required stream.
 * @return the correct stream or NULL if one is not found.
 */
static AVStream* getStream(const AVFormatContext *formatContext,
        const AVMediaType& mediaType) {

    AVStream *stream = NULL;
    AVCodecContext *codec = NULL;

    for (int i = 0; i < formatContext->nb_streams; i++) {

        stream = formatContext->streams[i];
        // If there is no stream at this index try the next.
        if (NULL == stream) continue;

        codec = stream->codec;
        // If this stream does not have a codec try the next.
        if (NULL == codec) continue;

        if (mediaType == codec->codec_type) return stream;
    }

    return NULL;
}

/**
 * Get the codec from the supplied format context that matches the
 * supplied media type.
 *
 * @param formatContext - the format context to get the codec from.
 * @param mediaType - the media type of the required codec.
 * @return the correct codec or NULL if one is not found.
 */
static AVCodecContext* getCodecContext(const AVFormatContext *formatContext,
        const AVMediaType& mediaType) {

    AVStream *stream = getStream(formatContext, mediaType);

    return NULL == stream ? NULL : stream->codec;
}

/**
 * Get the packet from the supplied format context that matches the
 * supplied media type.
 *
 * @param formatContext - the format context to get the packet from.
 * @param mediaType - the media type of the required packet.
 * @return the correct packet or NULL if one is not found within the
 *      first 1000 packets.
 */
static AVPacket* getPacket(AVFormatContext *formatContext,
        const AVMediaType& mediaType) {

    // Find the stream of the type we want, it's index will be cross
    // referenced against the packets stream index.
    AVStream *stream = getStream(formatContext, mediaType);

    AVPacket *packet = new AVPacket();

    av_init_packet(packet);

    // Only try 1000 packets, we don't want to read the whole file.
    for (int i = 0; i < 1000; i++) {

        av_read_frame(formatContext, packet);

        if (stream->index == packet->stream_index) return packet;

        // If this isn't the right packet type discard it.
        av_free_packet(packet);
    }

    return NULL;
}

/**
 * This function will retry decoding an audio up to five times, this
 * is done because some codecs first packet isn't decodable.
 *
 * @param audioPacket - the first packet to try decoding.
 * @param formatContext - the format context to use in the decoding of the packet.
 */
static std::vector<AVFrame*> retryDecodeAudioFrame(AVPacket *audioPacket,
        AVFormatContext *formatContext) {

    std::vector<AVFrame*> audioFrames;

    // Try to decode an audio frame from one of the first five packets.
    for (int i = 0; i < 5; i++) {

        audioFrames = transcode::util::decodeAudioFrame(audioPacket,
                formatContext);

        // If we get a frame return it.
        if (0 < audioFrames.size()) return audioFrames;

        av_free_packet(audioPacket);
        for (int i = 0; i < audioFrames.size(); i++) av_free(audioFrames[i]);

        audioPacket = getPacket(formatContext, AVMEDIA_TYPE_AUDIO);
    }

    return audioFrames;
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
            FormatContextFixture() {

        formatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
    }
};

struct MKVFormatContextFixture: public FormatContextFixture {

    MKVFormatContextFixture() :
            FormatContextFixture() {

        formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
    }
};

struct MP4FormatContextFixture: public FormatContextFixture {

    MP4FormatContextFixture() :
            FormatContextFixture() {

        formatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
    }
};

struct OGVFormatContextFixture: public FormatContextFixture {

    OGVFormatContextFixture() :
            FormatContextFixture() {

        formatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
    }
};

struct FLVFormatContextFixture: public FormatContextFixture {

    FLVFormatContextFixture() :
            FormatContextFixture() {

        formatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
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

struct MultiInitialisedFormatContextFixture: public MultiFormatContextFixture {

    MultiInitialisedFormatContextFixture() :
            MultiFormatContextFixture() {

        aviFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_AVI);
        mkvFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_MKV);
        mp4FormatContext = transcode::util::retrieveAVFormatContext(VIDEO_MP4);
        ogvFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_OGV);
        flvFormatContext = transcode::util::retrieveAVFormatContext(VIDEO_FLV);
    }

    virtual ~MultiInitialisedFormatContextFixture() {
    }
};

struct PacketFixture {

    PacketFixture() : packet(NULL) {}

    ~PacketFixture() {

        av_free_packet(packet);
    }

    AVPacket *packet;
};

struct AVINullPacketFixture : public AVIFormatContextFixture, PacketFixture {

    AVINullPacketFixture() : AVIFormatContextFixture(), PacketFixture() {}
};

struct AVIPacketFixture : public AVIFormatContextFixture, PacketFixture {

    AVIPacketFixture() : AVIFormatContextFixture(), PacketFixture() {

        packet = transcode::util::readNextPacket(formatContext);
    }
};

struct MKVNullPacketFixture : public MKVFormatContextFixture, PacketFixture {

    MKVNullPacketFixture() : MKVFormatContextFixture(), PacketFixture() {}
};

struct MKVPacketFixture : public MKVFormatContextFixture, PacketFixture {

    MKVPacketFixture() : MKVFormatContextFixture(), PacketFixture() {

        packet = transcode::util::readNextPacket(formatContext);
    }
};

struct MP4NullPacketFixture : public MP4FormatContextFixture, PacketFixture {

    MP4NullPacketFixture() : MP4FormatContextFixture(), PacketFixture() {}
};

struct MP4PacketFixture : public MP4FormatContextFixture, PacketFixture {

    MP4PacketFixture() : MP4FormatContextFixture(), PacketFixture() {

        packet = transcode::util::readNextPacket(formatContext);
    }
};

struct OGVNullPacketFixture : public OGVFormatContextFixture, PacketFixture {

    OGVNullPacketFixture() : OGVFormatContextFixture(), PacketFixture() {}
};

struct OGVPacketFixture : public OGVFormatContextFixture, PacketFixture {

    OGVPacketFixture() : OGVFormatContextFixture(), PacketFixture() {

        packet = transcode::util::readNextPacket(formatContext);
    }
};

struct FLVNullPacketFixture : public FLVFormatContextFixture, PacketFixture {

    FLVNullPacketFixture() : FLVFormatContextFixture(), PacketFixture() {}
};

struct FLVPacketFixture : public FLVFormatContextFixture, PacketFixture {

    FLVPacketFixture() : FLVFormatContextFixture(), PacketFixture() {

        packet = transcode::util::readNextPacket(formatContext);
    }
};

struct MultiPacketFixture: public MultiInitialisedFormatContextFixture {

    MultiPacketFixture() :
            MultiInitialisedFormatContextFixture(),
                    aviPacket(NULL), mkvPacket(NULL), mp4Packet(NULL),
                    ogvPacket(NULL), flvPacket(NULL)
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

struct MultiInitialisedPacketFixture: public MultiPacketFixture {

    MultiInitialisedPacketFixture() :
            MultiPacketFixture() {

        aviPacket = transcode::util::readNextPacket(aviFormatContext);
        mkvPacket = transcode::util::readNextPacket(mkvFormatContext);
        mp4Packet = transcode::util::readNextPacket(mp4FormatContext);
        ogvPacket = transcode::util::readNextPacket(ogvFormatContext);
        flvPacket = transcode::util::readNextPacket(flvFormatContext);
    }
};

struct CodecContextFixture {

    CodecContextFixture() : videoCodec(NULL), audioCodec(NULL) {
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

struct AVICodecContextFixture: public AVIFormatContextFixture, CodecContextFixture {

    AVICodecContextFixture() : AVIFormatContextFixture(), CodecContextFixture() {

        audioCodec = getCodecContext(formatContext, AVMEDIA_TYPE_AUDIO);
        videoCodec = getCodecContext(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct MKVCodecContextFixture: public MKVFormatContextFixture, CodecContextFixture {

    MKVCodecContextFixture() : MKVFormatContextFixture(), CodecContextFixture() {

        audioCodec = getCodecContext(formatContext, AVMEDIA_TYPE_AUDIO);
        videoCodec = getCodecContext(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct MP4CodecContextFixture: public MP4FormatContextFixture, CodecContextFixture {

    MP4CodecContextFixture() : MP4FormatContextFixture(), CodecContextFixture() {

        audioCodec = getCodecContext(formatContext, AVMEDIA_TYPE_AUDIO);
        videoCodec = getCodecContext(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct OGVCodecContextFixture: public OGVFormatContextFixture, CodecContextFixture {

    OGVCodecContextFixture() : OGVFormatContextFixture(), CodecContextFixture() {

        audioCodec = getCodecContext(formatContext, AVMEDIA_TYPE_AUDIO);
        videoCodec = getCodecContext(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct FLVCodecContextFixture: public FLVFormatContextFixture, CodecContextFixture {

    FLVCodecContextFixture() : FLVFormatContextFixture(), CodecContextFixture() {

        audioCodec = getCodecContext(formatContext, AVMEDIA_TYPE_AUDIO);
        videoCodec = getCodecContext(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct MultiCodecContextFixture: public MultiInitialisedFormatContextFixture {

    MultiCodecContextFixture() :
            MultiInitialisedFormatContextFixture(),
                    aviAudioCodec(NULL), aviVideoCodec(NULL),
                    mkvAudioCodec(NULL), mkvVideoCodec(NULL),
                    mp4AudioCodec(NULL), mp4VideoCodec(NULL),
                    ogvAudioCodec(NULL), ogvVideoCodec(NULL),
                    flvAudioCodec(NULL), flvVideoCodec(NULL)
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
            MultiCodecContextFixture() {

        aviAudioCodec = getCodecContext(aviFormatContext, AVMEDIA_TYPE_AUDIO);
        aviVideoCodec = getCodecContext(aviFormatContext, AVMEDIA_TYPE_VIDEO);
        mkvAudioCodec = getCodecContext(mkvFormatContext, AVMEDIA_TYPE_AUDIO);
        mkvVideoCodec = getCodecContext(mkvFormatContext, AVMEDIA_TYPE_VIDEO);
        mp4AudioCodec = getCodecContext(mp4FormatContext, AVMEDIA_TYPE_AUDIO);
        mp4VideoCodec = getCodecContext(mp4FormatContext, AVMEDIA_TYPE_VIDEO);
        ogvAudioCodec = getCodecContext(ogvFormatContext, AVMEDIA_TYPE_AUDIO);
        ogvVideoCodec = getCodecContext(ogvFormatContext, AVMEDIA_TYPE_VIDEO);
        flvAudioCodec = getCodecContext(flvFormatContext, AVMEDIA_TYPE_AUDIO);
        flvVideoCodec = getCodecContext(flvFormatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct FramesFixture {

    FramesFixture() :
            audioPacket(NULL), videoPacket(NULL),
                    audioFrames(std::vector<AVFrame*>()), videoFrame(NULL) {
    }

    virtual ~FramesFixture() {

        av_free_packet(audioPacket);
        av_free_packet(videoPacket);
        for (int i = 0; i < audioFrames.size(); i++) av_free(audioFrames[i]);
        av_free(videoFrame);
    }

    AVPacket *audioPacket;
    AVPacket *videoPacket;

    std::vector<AVFrame*> audioFrames;
    AVFrame *videoFrame;
};

struct AVIFramesFixture: public AVIFormatContextFixture, FramesFixture {

    AVIFramesFixture() :
            AVIFormatContextFixture(), FramesFixture() {

        audioPacket = getPacket(formatContext, AVMEDIA_TYPE_AUDIO);
        videoPacket = getPacket(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct AVIInitialisedFramesFixture: public AVIFramesFixture {

    AVIInitialisedFramesFixture() :
            AVIFramesFixture() {

        audioFrames = transcode::util::decodeAudioFrame(audioPacket,
                formatContext);
        videoFrame = transcode::util::decodeVideoFrame(videoPacket,
                formatContext);
    }
};

struct MKVFramesFixture: public MKVFormatContextFixture, FramesFixture {

    MKVFramesFixture() :
            MKVFormatContextFixture(), FramesFixture() {

        audioPacket = getPacket(formatContext, AVMEDIA_TYPE_AUDIO);
        videoPacket = getPacket(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct MKVInitialisedFramesFixture: public MKVFramesFixture {

    MKVInitialisedFramesFixture() :
            MKVFramesFixture() {

        audioFrames = transcode::util::decodeAudioFrame(audioPacket,
                formatContext);
        videoFrame = transcode::util::decodeVideoFrame(videoPacket,
                formatContext);
    }
};

struct MP4FramesFixture: public MP4FormatContextFixture, FramesFixture {

    MP4FramesFixture() :
            MP4FormatContextFixture(), FramesFixture() {

        audioPacket = getPacket(formatContext, AVMEDIA_TYPE_AUDIO);
        videoPacket = getPacket(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct MP4InitialisedFramesFixture: public MP4FramesFixture {

    MP4InitialisedFramesFixture() :
            MP4FramesFixture() {

        audioFrames = transcode::util::decodeAudioFrame(audioPacket,
                formatContext);
        videoFrame = transcode::util::decodeVideoFrame(videoPacket,
                formatContext);
    }
};

struct OGVFramesFixture: public OGVFormatContextFixture, FramesFixture {

    OGVFramesFixture() :
            OGVFormatContextFixture(), FramesFixture() {

        audioPacket = getPacket(formatContext, AVMEDIA_TYPE_AUDIO);
        videoPacket = getPacket(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct OGVInitialisedFramesFixture: public OGVFramesFixture {

    OGVInitialisedFramesFixture() :
            OGVFramesFixture() {

        audioFrames = transcode::util::decodeAudioFrame(audioPacket,
                formatContext);
        videoFrame = transcode::util::decodeVideoFrame(videoPacket,
                formatContext);
    }
};

struct FLVFramesFixture: public FLVFormatContextFixture, FramesFixture {

    FLVFramesFixture() :
            FLVFormatContextFixture(), FramesFixture() {

        audioPacket = getPacket(formatContext, AVMEDIA_TYPE_AUDIO);
        videoPacket = getPacket(formatContext, AVMEDIA_TYPE_VIDEO);
    }
};

struct FLVInitialisedFramesFixture: public FLVFramesFixture {

    FLVInitialisedFramesFixture() :
            FLVFramesFixture() {

        audioFrames = transcode::util::decodeAudioFrame(audioPacket,
                formatContext);
        videoFrame = transcode::util::decodeVideoFrame(videoPacket,
                formatContext);
    }
};

struct MultiFramesFixture: public MultiInitialisedFormatContextFixture {

    MultiFramesFixture() :
            MultiInitialisedFormatContextFixture(),
                    aviAudioPacket(
                            getPacket(aviFormatContext, AVMEDIA_TYPE_AUDIO)),
                    aviVideoPacket(
                            getPacket(aviFormatContext, AVMEDIA_TYPE_VIDEO)),
                    mkvAudioPacket(
                            getPacket(mkvFormatContext, AVMEDIA_TYPE_AUDIO)),
                    mkvVideoPacket(
                            getPacket(mkvFormatContext, AVMEDIA_TYPE_VIDEO)),
                    mp4AudioPacket(
                            getPacket(mp4FormatContext, AVMEDIA_TYPE_AUDIO)),
                    mp4VideoPacket(
                            getPacket(mp4FormatContext, AVMEDIA_TYPE_VIDEO)),
                    ogvAudioPacket(
                            getPacket(ogvFormatContext, AVMEDIA_TYPE_AUDIO)),
                    ogvVideoPacket(
                            getPacket(ogvFormatContext, AVMEDIA_TYPE_VIDEO)),
                    flvAudioPacket(
                            getPacket(flvFormatContext, AVMEDIA_TYPE_AUDIO)),
                    flvVideoPacket(
                            getPacket(flvFormatContext, AVMEDIA_TYPE_VIDEO)),
                    aviAudioFrames(std::vector<AVFrame*>()),
                    aviVideoFrame(NULL),
                    mkvAudioFrames(std::vector<AVFrame*>()),
                    mkvVideoFrame(NULL),
                    mp4AudioFrames(std::vector<AVFrame*>()),
                    mp4VideoFrame(NULL),
                    ogvAudioFrames(std::vector<AVFrame*>()),
                    ogvVideoFrame(NULL),
                    flvAudioFrames(std::vector<AVFrame*>()),
                    flvVideoFrame(NULL)
    {
    }

    virtual ~MultiFramesFixture() {

        av_free_packet(aviAudioPacket);
        av_free_packet(aviVideoPacket);
        av_free_packet(mkvAudioPacket);
        av_free_packet(mkvVideoPacket);
        av_free_packet(mp4AudioPacket);
        av_free_packet(mp4VideoPacket);
        av_free_packet(ogvAudioPacket);
        av_free_packet(ogvVideoPacket);
        av_free_packet(flvAudioPacket);
        av_free_packet(flvVideoPacket);
        for (int i = 0; i < aviAudioFrames.size(); i++) av_free(aviAudioFrames[i]);
        av_free(aviVideoFrame);
        for (int i = 0; i < mkvAudioFrames.size(); i++) av_free(mkvAudioFrames[i]);
        av_free(mkvVideoFrame);
        for (int i = 0; i < mp4AudioFrames.size(); i++) av_free(mp4AudioFrames[i]);
        av_free(mp4VideoFrame);
        for (int i = 0; i < ogvAudioFrames.size(); i++) av_free(ogvAudioFrames[i]);
        av_free(ogvVideoFrame);
        for (int i = 0; i < flvAudioFrames.size(); i++) av_free(flvAudioFrames[i]);
        av_free(flvVideoFrame);
    }

    AVPacket *aviAudioPacket;
    AVPacket *aviVideoPacket;
    AVPacket *mkvAudioPacket;
    AVPacket *mkvVideoPacket;
    AVPacket *mp4AudioPacket;
    AVPacket *mp4VideoPacket;
    AVPacket *ogvAudioPacket;
    AVPacket *ogvVideoPacket;
    AVPacket *flvAudioPacket;
    AVPacket *flvVideoPacket;

    std::vector<AVFrame*> aviAudioFrames;
    AVFrame *aviVideoFrame;
    std::vector<AVFrame*> mkvAudioFrames;
    AVFrame *mkvVideoFrame;
    std::vector<AVFrame*> mp4AudioFrames;
    AVFrame *mp4VideoFrame;
    std::vector<AVFrame*> ogvAudioFrames;
    AVFrame *ogvVideoFrame;
    std::vector<AVFrame*> flvAudioFrames;
    AVFrame *flvVideoFrame;
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

    aviAudioCodec = transcode::util::openCodecContext(aviAudioCodec);
    aviVideoCodec = transcode::util::openCodecContext(aviVideoCodec);
    mkvAudioCodec = transcode::util::openCodecContext(mkvAudioCodec);
    mkvVideoCodec = transcode::util::openCodecContext(mkvVideoCodec);
    mp4AudioCodec = transcode::util::openCodecContext(mp4AudioCodec);
    mp4VideoCodec = transcode::util::openCodecContext(mp4VideoCodec);
    ogvAudioCodec = transcode::util::openCodecContext(ogvAudioCodec);
    ogvVideoCodec = transcode::util::openCodecContext(ogvVideoCodec);
    flvAudioCodec = transcode::util::openCodecContext(flvAudioCodec);
    flvVideoCodec = transcode::util::openCodecContext(flvVideoCodec);
}

/**
 * Test to make sure an exception is thrown if a NULL codec context is opened.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_open_null_codec )
{

    BOOST_REQUIRE_THROW( transcode::util::openCodecContext(NULL),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure an exception is thrown if a blank codec context is opened.
 */
BOOST_AUTO_TEST_CASE( test_ffmpeg_open_empty_codec )
{

    BOOST_REQUIRE_THROW( transcode::util::openCodecContext(&blankCodecContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure that an audio packet can be decoded from all the differnet media files.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_audio_packet, MultiFramesFixture )
{

    aviAudioFrames = transcode::util::decodeAudioFrame(aviAudioPacket, aviFormatContext);
    mkvAudioFrames = transcode::util::decodeAudioFrame(mkvAudioPacket, mkvFormatContext);
    mp4AudioFrames = transcode::util::decodeAudioFrame(mp4AudioPacket, mp4FormatContext);
    ogvAudioFrames = transcode::util::decodeAudioFrame(ogvAudioPacket, ogvFormatContext);
    flvAudioFrames = transcode::util::decodeAudioFrame(flvAudioPacket, flvFormatContext);
}

/**
 * Test to make sure an exception is thrown if a NULL packet is decoded.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_null_audio_packet, AVIFormatContextFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::decodeAudioFrame(NULL, formatContext),
            transcode::util::FFMPEGException);
}

/**
 * Test to make sure an exception is thrown if a blank packet is decoded.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_blank_audio_packet, AVIFormatContextFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::decodeAudioFrame(&blankPacket, formatContext),
                transcode::util::FFMPEGException);
}

/**
 * Test to make sure an exception is thrown if a blank packet is decoded.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_audio_packet_with_null_format_context,
        AVIFramesFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::decodeAudioFrame(audioPacket, NULL),
                transcode::util::FFMPEGException);
}

/**
 * Test to make sure an exception is thrown if a blank packet is decoded.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_audio_packet_with_blank_format_context,
        AVIFramesFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::decodeAudioFrame(audioPacket,
            &blankFormatContext), transcode::util::FFMPEGException);
}

/**
 * Test to make sure the an audio packet can be decoded from an avi media file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_avi_audio_packet, AVIFramesFixture )
{

    audioFrames = transcode::util::decodeAudioFrame(audioPacket, formatContext);

    BOOST_REQUIRE( 0 < audioFrames.size() );
}

/**
 * Test to make sure the an audio packet can be decoded from an mkv media file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_mkv_audio_packet, MKVFramesFixture )
{

    audioFrames = transcode::util::decodeAudioFrame(audioPacket, formatContext);

    BOOST_REQUIRE( 0 < audioFrames.size() );
}

/**
 * Test to make sure the an audio packet can be decoded from an mp4 media file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_mp4_audio_packet, MP4FramesFixture )
{

    audioFrames = transcode::util::decodeAudioFrame(audioPacket, formatContext);

    BOOST_REQUIRE( 0 < audioFrames.size() );
}

/**
 * Test to make sure the an audio packet can be decoded from an ogv media file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_ogv_audio_packet, OGVFramesFixture )
{

    // Try five times to decode the vorbis stream because the first packet
    // doesn't seem to work.
    audioFrames = retryDecodeAudioFrame(audioPacket, formatContext);

    BOOST_REQUIRE( 0 < audioFrames.size() );
}

/**
 * Test to make sure the an audio packet can be decoded from an flv media file.
 */
BOOST_FIXTURE_TEST_CASE( test_ffmpeg_decode_flv_audio_packet, FLVFramesFixture )
{

    audioFrames = transcode::util::decodeAudioFrame(audioPacket, formatContext);

    BOOST_REQUIRE( 0 < audioFrames.size() );
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
