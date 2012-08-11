/*
 * libav_test.cpp
 *
 *  Created on: 9 May 2012
 *      Author: karl
 */

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <test_fixtures.hpp>
#include <util_test.hpp>

#include <libav/libav.hpp>
#include <libav/libaverror.hpp>

#include <iostream>


/**
 * Test error message success.
 */
BOOST_AUTO_TEST_CASE( test_error_message_success )
{

    BOOST_REQUIRE( transcode::libav::UNKNOWN !=
                transcode::libav::errorMessage(0) );
}

/**
 * Test error message unknown.
 */
BOOST_AUTO_TEST_CASE( test_error_message_unknown )
{

    BOOST_REQUIRE_EQUAL( transcode::libav::UNKNOWN,
                transcode::libav::errorMessage(1) );
}

/**
 * Test error message for AVERROR_INVALIDDATA.
 */
BOOST_AUTO_TEST_CASE( test_error_message_invalid_data )
{

    BOOST_REQUIRE_EQUAL( "Invalid data found when processing input",
                transcode::libav::errorMessage(AVERROR_INVALIDDATA) );
}

/**
 * Test open avi format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_avi, test::FormatContextFixture )
{

    formatContext = transcode::libav::openFormatContext(VIDEO_AVI);
}

/**
 * Test open mkv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_mkv, test::FormatContextFixture )
{

    formatContext = transcode::libav::openFormatContext(VIDEO_MKV);
}

/**
 * Test open ogv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_ogv, test::FormatContextFixture )
{

    formatContext = transcode::libav::openFormatContext(VIDEO_OGV);
}

/**
 * Test open mp4 format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_mp4, test::FormatContextFixture )
{

    formatContext = transcode::libav::openFormatContext(VIDEO_MP4);
}

/**
 * Test open flv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_flv, test::FormatContextFixture )
{

    formatContext = transcode::libav::openFormatContext(VIDEO_FLV);
}

/**
 * Test open text format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_text_file, test::FormatContextFixture )
{

    formatContext = transcode::libav::openFormatContext(TEXT_FILE);
}

/**
 * Test open empty format context.
 */
BOOST_AUTO_TEST_CASE( test_open_format_context_for_empty_file )
{

    BOOST_REQUIRE_THROW( transcode::libav::openFormatContext(EMPTY_FILE),
            transcode::IOException );
}

/**
 * Test close avi format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_avi, test::AVIFormatContextFixture )
{

     transcode::libav::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close mkv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_mkv, test::MKVFormatContextFixture )
{

     transcode::libav::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close ogv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_ogv, test::OGVFormatContextFixture )
{

     transcode::libav::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close mp4 format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_mp4, test::MP4FormatContextFixture )
{

     transcode::libav::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close flv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_flv, test::FLVFormatContextFixture )
{

     transcode::libav::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close empty format context.
 */
BOOST_AUTO_TEST_CASE( test_close_empty_format_context )
{

    AVFormatContext *formatContext = new AVFormatContext();

    BOOST_REQUIRE_THROW( transcode::libav::closeFormatContext(&formatContext),
               transcode::IllegalStateException );

    delete(formatContext);
}

/**
 * Test close NULL format context.
 */
BOOST_AUTO_TEST_CASE( test_close_null_format_context )
{

    AVFormatContext *formatContext = NULL;

    BOOST_REQUIRE_THROW( transcode::libav::closeFormatContext(&formatContext),
               transcode::IllegalArgumentException );
}

/**
 * Test read packet from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_avi_file, test::AVIFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::libav::readNextPacket(formatContext) );
}

/**
 * Test read packet from a mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_mkv_file, test::MKVFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::libav::readNextPacket(formatContext) );
}

/**
 * Test read packet from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_ogv_file, test::OGVFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::libav::readNextPacket(formatContext) );
}

/**
 * Test read packet from a mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_mp4_file, test::MP4FormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::libav::readNextPacket(formatContext) );
}

/**
 * Test read packet from a flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_flv_file, test::FLVFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::libav::readNextPacket(formatContext) );
}

/**
 * Test read packet from a text file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_text_file, test::TextFormatContextFixture )
{

    BOOST_REQUIRE( NULL != transcode::libav::readNextPacket(formatContext) );
}

/**
 * Test read packet from an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_read_packet_from_empty_format_context )
{

    AVFormatContext formatContext;
    formatContext.nb_streams = 0; // Only way to guaranty a packet read failure.

    BOOST_REQUIRE_THROW( transcode::libav::readNextPacket(&formatContext),
            transcode::IllegalStateException );
}

/**
 * Test read packet from a NULL format context.
 */
BOOST_AUTO_TEST_CASE( test_read_packet_from_null_format_context )
{

    BOOST_REQUIRE_THROW( transcode::libav::readNextPacket((AVFormatContext*) NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test find stream type from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_avi_file, test::AVIStreamFixture )
{

    BOOST_REQUIRE_EQUAL( DIVX_STREAM_ONE, transcode::libav::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( DIVX_STREAM_TWO, transcode::libav::findStreamType(streams[1]) );
}

/**
 * Test find stream type from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_mkv_file, test::AVIStreamFixture )
{

    BOOST_REQUIRE_EQUAL( MKV_STREAM_ONE, transcode::libav::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( MKV_STREAM_TWO, transcode::libav::findStreamType(streams[1]) );
}

/**
 * Test find stream type from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_ogv_file, test::OGVStreamFixture )
{

    BOOST_REQUIRE_EQUAL( OGV_STREAM_ONE, transcode::libav::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_TWO, transcode::libav::findStreamType(streams[1]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_THREE, transcode::libav::findStreamType(streams[2]) );
}

/**
 * Test find stream type from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_mp4_file, test::MP4StreamFixture )
{

    BOOST_REQUIRE_EQUAL( MP4_STREAM_ONE, transcode::libav::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( MP4_STREAM_TWO, transcode::libav::findStreamType(streams[1]) );
}

/**
 * Test find stream type from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_flv_file, test::FLVStreamFixture )
{

    BOOST_REQUIRE_EQUAL( FLV_STREAM_ONE, transcode::libav::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( FLV_STREAM_TWO, transcode::libav::findStreamType(streams[1]) );
}

/**
 * Test find type for an empty stream.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_empty_stream )
{

    AVStream stream;
    stream.codec = NULL; // Only way for this test to be ok.

    BOOST_REQUIRE_THROW( transcode::libav::findStreamType(&stream),
            transcode::IllegalArgumentException );
}

/**
 * Test find type for a null stream.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_null_stream )
{

    BOOST_REQUIRE_THROW( transcode::libav::findStreamType(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test find codec type from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_avi_file, test::AVICodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( DIVX_STREAM_ONE, transcode::libav::findCodecType(decodeCodecs[0]) );
    BOOST_REQUIRE_EQUAL( DIVX_STREAM_TWO, transcode::libav::findCodecType(decodeCodecs[1]) );
}

/**
 * Test find codec type from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_mkv_file, test::MKVCodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( MKV_STREAM_ONE, transcode::libav::findCodecType(decodeCodecs[0]) );
    BOOST_REQUIRE_EQUAL( MKV_STREAM_TWO, transcode::libav::findCodecType(decodeCodecs[1]) );
}

/**
 * Test find codec type from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_ogv_file, test::OGVCodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( OGV_STREAM_ONE, transcode::libav::findCodecType(decodeCodecs[0]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_TWO, transcode::libav::findCodecType(decodeCodecs[1]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_THREE, transcode::libav::findCodecType(decodeCodecs[2]) );
}

/**
 * Test find codec type from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_mp4_file, test::MP4CodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( MP4_STREAM_ONE, transcode::libav::findCodecType(decodeCodecs[0]) );
    BOOST_REQUIRE_EQUAL( MP4_STREAM_TWO, transcode::libav::findCodecType(decodeCodecs[1]) );
}

/**
 * Test find codec type from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_flv_file, test::FLVCodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( FLV_STREAM_ONE, transcode::libav::findCodecType(decodeCodecs[0]) );
    BOOST_REQUIRE_EQUAL( FLV_STREAM_TWO, transcode::libav::findCodecType(decodeCodecs[1]) );
}

/**
 * Test find type for an empty codec.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_empty_codec )
{

    AVCodecContext codec;

    BOOST_REQUIRE_THROW( transcode::libav::findCodecType(&codec),
            transcode::IllegalStateException );
}

/**
 * Test find type for a null codec.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_null_codec )
{

    BOOST_REQUIRE_THROW( transcode::libav::findCodecType(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test find packet type from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_avi_file, test::AVIPacketFixture )
{

    BOOST_REQUIRE_EQUAL( DIVX_STREAM_ONE, transcode::libav::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an mkvfile.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_mkv_file, test::MKVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( MKV_STREAM_ONE, transcode::libav::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_ogv_file, test::OGVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( OGV_STREAM_THREE, transcode::libav::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_mp4_file, test::MP4PacketFixture )
{

    BOOST_REQUIRE_EQUAL( MP4_STREAM_TWO, transcode::libav::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_flv_file, test::FLVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( FLV_STREAM_TWO, transcode::libav::findPacketType(formatContext, packet) );
}

/**
 * Test find type for a null format context.
 */
BOOST_FIXTURE_TEST_CASE( test_find_type_for_null_format_context, test::AVIPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::findPacketType(NULL, packet),
            transcode::IllegalArgumentException );
}

/**
 * Test find type for a null packet.
 */
BOOST_FIXTURE_TEST_CASE( test_find_type_for_null_packet, test::AVIFormatContextFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::findPacketType(formatContext, NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test open decode codecs for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_decode_codecs_for_avi_file, test::AVICodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open decode codecs for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_decode_codecs_for_mkv_file, test::MKVCodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open decode codecs for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_decode_codecs_for_ogv_file, test::OGVCodecContextFixture )
{
// TODO: Subtitle codecs not yet supported.
//    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(codecs[0]) );
    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[1]) );
    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[2]) );
}

/**
 * Test open decode codecs for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_decode_codecs_for_mp4_file, test::MP4CodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open decode codecs for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_decode_codecs_for_flv_file, test::FLVCodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openDecodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open decode codec for a null codec.
 */
BOOST_AUTO_TEST_CASE( test_open_decode_codec_for_null_codec )
{

    BOOST_REQUIRE_THROW( transcode::libav::openDecodeCodecContext(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test open encode codecs for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_encode_codecs_for_avi_file, test::AVICodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open encode codecs for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_encode_codecs_for_mkv_file, test::MKVCodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open encode codecs for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_encode_codecs_for_ogv_file, test::OGVCodecContextFixture )
{
// TODO: Subtitle codecs not yet supported.
//    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(codecs[0]) );
    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[1]) );
    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[2]) );
}

/**
 * Test open encode codecs for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_encode_codecs_for_mp4_file, test::MP4CodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open encode codecs for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_encode_codecs_for_flv_file, test::FLVCodecContextFixture )
{

    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[0]) );
    BOOST_REQUIRE( transcode::libav::openEncodeCodecContext(decodeCodecs[1]) );
}

/**
 * Test open encode codec for a null codec.
 */
BOOST_AUTO_TEST_CASE( test_open_encode_codec_for_null_codec )
{

    BOOST_REQUIRE_THROW( transcode::libav::openEncodeCodecContext(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test close codecs for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_avi_file, test::AVIOpenedCodecContextFixture )
{

    transcode::libav::closeCodecContext(&(decodeCodecs[0]));
    transcode::libav::closeCodecContext(&(decodeCodecs[1]));
    transcode::libav::closeCodecContext(&(encodeCodecs[0]));
    transcode::libav::closeCodecContext(&(encodeCodecs[1]));
}

/**
 * Test close codecs for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_mkv_file, test::MKVOpenedCodecContextFixture )
{
    transcode::libav::closeCodecContext(&(decodeCodecs[0]));
    transcode::libav::closeCodecContext(&(decodeCodecs[1]));
    transcode::libav::closeCodecContext(&(encodeCodecs[0]));
    transcode::libav::closeCodecContext(&(encodeCodecs[1]));
}

/**
 * Test close codecs for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_ogv_file, test::OGVOpenedCodecContextFixture )
{

    transcode::libav::closeCodecContext(&(decodeCodecs[0]));
    transcode::libav::closeCodecContext(&(decodeCodecs[1]));
    transcode::libav::closeCodecContext(&(decodeCodecs[2]));
    // TODO: Subtitle streams are not yet supported.
    //transcode::libav::closeCodecContext(&(decodeCodecs[0]));
    transcode::libav::closeCodecContext(&(encodeCodecs[1]));
    transcode::libav::closeCodecContext(&(encodeCodecs[2]));
}

/**
 * Test close codecs for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_mp4_file, test::MP4OpenedCodecContextFixture )
{

    transcode::libav::closeCodecContext(&(decodeCodecs[0]));
    transcode::libav::closeCodecContext(&(decodeCodecs[1]));
    transcode::libav::closeCodecContext(&(encodeCodecs[0]));
    transcode::libav::closeCodecContext(&(encodeCodecs[1]));
}

/**
 * Test close codecs for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_flv_file, test::FLVOpenedCodecContextFixture )
{

    transcode::libav::closeCodecContext(&(decodeCodecs[0]));
    transcode::libav::closeCodecContext(&(decodeCodecs[1]));
    transcode::libav::closeCodecContext(&(encodeCodecs[0]));
    transcode::libav::closeCodecContext(&(encodeCodecs[1]));
}

/**
 * Test close codec for a null codec pointer.
 */
BOOST_AUTO_TEST_CASE( test_close_codec_for_null_codec_pointer )
{

    BOOST_REQUIRE_THROW( transcode::libav::closeCodecContext(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test close codec for a null codec.
 */
BOOST_AUTO_TEST_CASE( test_close_codec_for_null_codec )
{

    AVCodecContext *codecs[1];
    codecs[0] = NULL;

    BOOST_REQUIRE_THROW( transcode::libav::closeCodecContext(codecs),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_avi_file, test::AVIAudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::libav::decodeAudioPacket(decodeCodecs[packet->stream_index], packet);

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_mkv_file, test::MKVAudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::libav::decodeAudioPacket(decodeCodecs[packet->stream_index], packet);

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_ogv_file, test::OGVAudioPacketFixture )
{

    std::vector<AVFrame*> frames;

    // Try a few times to get a valid vorbis frame because the first few packets aren't always valid.
    for (int i = 0; i < 5; i++) {

        frames = transcode::libav::decodeAudioPacket(decodeCodecs[packet->stream_index], packet);

        // If the first audio packet doesn't decode read in the next one audio packet.
        if (0 < frames.size()) break;

        av_free_packet(packet);

        packet = readPacket(AVMEDIA_TYPE_AUDIO);
    }

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_mp4_file, test::MP4AudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::libav::decodeAudioPacket(decodeCodecs[packet->stream_index], packet);

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_flv_file, test::FLVAudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::libav::decodeAudioPacket(decodeCodecs[packet->stream_index], packet);

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test audio decode video packet.
 */
BOOST_FIXTURE_TEST_CASE( test_audio_decode_video_packet, test::AVIVideoPacketFixture )
{

    AVPacket *audioPacket = readPacket(AVMEDIA_TYPE_AUDIO);

    AVCodecContext *codec = decodeCodecs[audioPacket->stream_index];

    av_free_packet(audioPacket);

    BOOST_REQUIRE_THROW( transcode::libav::decodeAudioPacket(codec, packet),
                transcode::libav::InvalidPacketDataException );
}

/**
 * Test decode audio packet with null codec.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_with_null_codec, test::AVIAudioPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::decodeAudioPacket(NULL, packet),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet with null packet.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_with_null_packet, test::AVIAudioPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::decodeAudioPacket(decodeCodecs[packet->stream_index], NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet with null codec and packet.
 */
BOOST_AUTO_TEST_CASE( test_decode_audio_packet_with_null_codec_and_packet )
{

    BOOST_REQUIRE_THROW( transcode::libav::decodeAudioPacket(NULL, NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test encode audio frame for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_audio_frame_for_avi_file, test::AVIAudioFrameFixture )
{

    AVPacket *audioPacket = retryEncodeFrameWrapper(transcode::libav::encodeAudioFrame);

    BOOST_REQUIRE( NULL != audioPacket );
}

/**
 * Test encode audio frame for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_audio_frame_for_mkv_file, test::MKVAudioFrameFixture )
{

    AVPacket *audioPacket = retryEncodeFrameWrapper(transcode::libav::encodeAudioFrame);

    BOOST_REQUIRE( NULL != audioPacket );
}

/**
 * Test encode audio frame for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_audio_frame_for_ogv_file, test::OGVAudioFrameFixture )
{

    AVPacket *audioPacket = retryEncodeFrameWrapper(transcode::libav::encodeAudioFrame);

    BOOST_REQUIRE( NULL != audioPacket );
}

/**
 * Test encode audio frame for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_audio_frame_for_mp4_file, test::MP4AudioFrameFixture )
{

    AVPacket *audioPacket = retryEncodeFrameWrapper(transcode::libav::encodeAudioFrame);

    BOOST_REQUIRE( NULL != audioPacket );
}

/**
 * Test encode audio frame for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_audio_frame_for_flv_file, test::FLVAudioFrameFixture )
{

    AVPacket *audioPacket = retryEncodeFrameWrapper(transcode::libav::encodeAudioFrame);

    BOOST_REQUIRE( NULL != audioPacket );
}

/**
 * Test audio encode video frame.
 */
BOOST_FIXTURE_TEST_CASE( test_audio_encode_video_frame, test::AVIVideoFrameFixture )
{

    AVPacket *audioPacket = readPacket(AVMEDIA_TYPE_AUDIO);

    AVCodecContext *codec = encodeCodecs[audioPacket->stream_index];

    av_free_packet(audioPacket);

    // Unfortunately it seems perfectly fine to try and encode a video frame with an audio codec.
    audioPacket = transcode::libav::encodeAudioFrame(codec, frames[0]);

    av_free_packet(audioPacket);
}

/**
 * Test encode audio frame with null codec.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_audio_frame_with_null_codec, test::AVIAudioFrameFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::encodeAudioFrame(NULL, frames[0]),
            transcode::IllegalArgumentException );
}

/**
 * Test encode audio frame with null packet.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_audio_frame_with_null_packet, test::AVIAudioFrameFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::encodeAudioFrame(decodeCodecs[packet->stream_index], NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test encode audio frame with null codec and packet.
 */
BOOST_AUTO_TEST_CASE( test_encode_audio_frame_with_null_codec_and_packet )
{

    BOOST_REQUIRE_THROW( transcode::libav::encodeAudioFrame(NULL, NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test decode video packet for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_video_packet_for_avi_file, test::AVIVideoPacketFixture )
{

    AVFrame *frame = retryPacketDecodeWrapper<AVFrame*>(transcode::libav::decodeVideoPacket);

    BOOST_REQUIRE( NULL != frame );
}

/**
 * Test decode video packet for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_video_packet_for_mkv_file, test::MKVVideoPacketFixture )
{

    AVFrame *frame = retryPacketDecodeWrapper<AVFrame*>(transcode::libav::decodeVideoPacket);

    BOOST_REQUIRE( NULL != frame );
}

/**
 * Test decode video packet for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_video_packet_for_ogv_file, test::OGVVideoPacketFixture )
{

    AVFrame *frame = retryPacketDecodeWrapper<AVFrame*>(transcode::libav::decodeVideoPacket);

    BOOST_REQUIRE( NULL != frame );
}

/**
 * Test decode video packet for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_video_packet_for_mp4_file, test::MP4VideoPacketFixture )
{

    AVFrame *frame = retryPacketDecodeWrapper<AVFrame*>(transcode::libav::decodeVideoPacket);

    BOOST_REQUIRE( NULL != frame );
}

/**
 * Test decode video packet for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_video_packet_for_flv_file, test::FLVVideoPacketFixture )
{

    AVFrame *frame = retryPacketDecodeWrapper<AVFrame*>(transcode::libav::decodeVideoPacket);

    BOOST_REQUIRE( NULL != frame );
}

/**
 * Test video decode audio packet.
 */
BOOST_FIXTURE_TEST_CASE( test_video_decode_audio_packet, test::AVIAudioPacketFixture )
{

    AVPacket *videoPacket = readPacket(AVMEDIA_TYPE_VIDEO);

    AVCodecContext *codec = decodeCodecs[videoPacket->stream_index];

    av_free_packet(videoPacket);

    // Unfortunately it appears that some audio packets will successfully decode when
    // passed through a video codec. This means it is almost impossible to check that
    // the right type of packet has been sent to this method.
    transcode::libav::decodeVideoPacket(codec, packet);
}

/**
 * Test decode video packet with null codec.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_video_packet_with_null_codec, test::AVIVideoPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::decodeVideoPacket(NULL, packet),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet with null packet.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_video_packet_with_null_packet, test::AVIVideoPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::decodeVideoPacket(decodeCodecs[packet->stream_index], NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet with null codec and packet.
 */
BOOST_AUTO_TEST_CASE( test_decode_video_packet_with_null_codec_and_packet )
{

    BOOST_REQUIRE_THROW( transcode::libav::decodeVideoPacket(NULL, NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test encode video frame for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_video_frame_for_avi_file, test::AVIVideoFrameFixture )
{

    AVPacket *videoPacket = retryEncodeFrameWrapper(transcode::libav::encodeVideoFrame);

    BOOST_REQUIRE( NULL != videoPacket );
}

/**
 * Test encode video frame for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_video_frame_for_mkv_file, test::MKVVideoFrameFixture )
{

    AVPacket *videoPacket = retryEncodeFrameWrapper(transcode::libav::encodeVideoFrame);

    BOOST_REQUIRE( NULL != videoPacket );
}

/**
 * Test encode video frame for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_video_frame_for_ogv_file, test::OGVVideoFrameFixture )
{

    AVPacket *videoPacket = retryEncodeFrameWrapper(transcode::libav::encodeVideoFrame);

    BOOST_REQUIRE( NULL != videoPacket );
}

/**
 * Test encode video frame for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_video_frame_for_mp4_file, test::MP4VideoFrameFixture )
{

    AVPacket *videoPacket = retryEncodeFrameWrapper(transcode::libav::encodeVideoFrame);

    BOOST_REQUIRE( NULL != videoPacket );
}

/**
 * Test encode video frame for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_video_frame_for_flv_file, test::FLVVideoFrameFixture )
{

    AVPacket *videoPacket = retryEncodeFrameWrapper(transcode::libav::encodeVideoFrame);

    BOOST_REQUIRE( NULL != videoPacket );
}

/**
 * Test video encode audio frame.
 */
BOOST_FIXTURE_TEST_CASE( test_video_encode_audio_frame, test::AVIAudioFrameFixture )
{

    AVPacket *videoPacket = readPacket(AVMEDIA_TYPE_VIDEO);

    AVCodecContext *codec = encodeCodecs[videoPacket->stream_index];

    av_free_packet(videoPacket);

    // Unfortunately trying to encode an audio frame with a video codec just causes a memory access violation
    // which can't be handled grasfully.
//    BOOST_REQUIRE_THROW( transcode::libav::encodeVideoFrame(codec, frames[0]),
//                transcode::libav::InvalidPacketDataException );
}

/**
 * Test encode video frame with null codec.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_video_frame_with_null_codec, test::AVIVideoFrameFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::encodeVideoFrame(NULL, frames[0]),
            transcode::IllegalArgumentException );
}

/**
 * Test encode video frame with null packet.
 */
BOOST_FIXTURE_TEST_CASE( test_encode_video_frame_with_null_packet, test::AVIVideoFrameFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::encodeVideoFrame(decodeCodecs[packet->stream_index], NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test encode video frame with null codec and packet.
 */
BOOST_AUTO_TEST_CASE( test_encode_video_frame_with_null_codec_and_packet )
{

    BOOST_REQUIRE_THROW( transcode::libav::encodeVideoFrame(NULL, NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test resample audio frame for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_resample_audio_frame_for_avi_file, test::AVIAudioFrameFixture )
{

    BOOST_REQUIRE( NULL != transcode::libav::resampleAudioFrame(avresample_alloc_context(), AV_SAMPLE_FMT_S16, frames[0]));
}

/**
 * Test resample audio frame for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_resample_audio_frame_for_mkv_file, test::MKVAudioFrameFixture )
{

    BOOST_REQUIRE( NULL != transcode::libav::resampleAudioFrame(avresample_alloc_context(), AV_SAMPLE_FMT_S16, frames[0]));
}

/**
 * Test resample audio frame for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_resample_audio_frame_for_ogv_file, test::OGVAudioFrameFixture )
{

    BOOST_REQUIRE( NULL != transcode::libav::resampleAudioFrame(avresample_alloc_context(), AV_SAMPLE_FMT_S16, frames[0]));
}

/**
 * Test resample audio frame for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_resample_audio_frame_for_mp4_file, test::MP4AudioFrameFixture )
{

    BOOST_REQUIRE( NULL != transcode::libav::resampleAudioFrame(avresample_alloc_context(), AV_SAMPLE_FMT_S16, frames[0]));
}

/**
 * Test resample audio frame for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_resample_audio_frame_for_flv_file, test::FLVAudioFrameFixture )
{

    BOOST_REQUIRE( NULL != transcode::libav::resampleAudioFrame(avresample_alloc_context(), AV_SAMPLE_FMT_S16, frames[0]));
}

/**
 * Test audio resample video frame.
 */
BOOST_FIXTURE_TEST_CASE( test_audio_resample_video_frame, test::AVIVideoFrameFixture )
{

    transcode::libav::resampleAudioFrame(avresample_alloc_context(), AV_SAMPLE_FMT_S16, frames[0]);
}

/**
 * Test resample audio frame with null codec.
 */
BOOST_FIXTURE_TEST_CASE( test_resample_audio_frame_with_null_codec, test::AVIAudioFrameFixture )
{

    BOOST_REQUIRE_THROW( transcode::libav::resampleAudioFrame(NULL, AV_SAMPLE_FMT_S16, frames[0]),
            transcode::IllegalArgumentException );
}
