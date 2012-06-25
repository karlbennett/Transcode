/*
 * util_libav_test.cpp
 *
 *  Created on: 9 May 2012
 *      Author: karl
 */

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <test_fixtures.hpp>
#include <util_test.hpp>

#include <util/util_libav.hpp>

#include <iostream>

/**
 * Test error message success.
 */
BOOST_AUTO_TEST_CASE( test_error_message_success )
{

    BOOST_REQUIRE( transcode::util::UNKNOWN !=
                transcode::util::errorMessage(0) );
}

/**
 * Test error message unknown.
 */
BOOST_AUTO_TEST_CASE( test_error_message_unknown )
{

    BOOST_REQUIRE_EQUAL( transcode::util::UNKNOWN,
                transcode::util::errorMessage(1) );
}

/**
 * Test error message for AVERROR_INVALIDDATA.
 */
BOOST_AUTO_TEST_CASE( test_error_message_invalid_data )
{

    BOOST_REQUIRE_EQUAL( "Invalid data found when processing input",
                transcode::util::errorMessage(AVERROR_INVALIDDATA) );
}

/**
 * Test open avi format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_avi, test::FormatContextFixture )
{

    formatContext = transcode::util::openFormatContext(VIDEO_AVI);
}

/**
 * Test open mkv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_mkv, test::FormatContextFixture )
{

    formatContext = transcode::util::openFormatContext(VIDEO_MKV);
}

/**
 * Test open ogv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_ogv, test::FormatContextFixture )
{

    formatContext = transcode::util::openFormatContext(VIDEO_OGV);
}

/**
 * Test open mp4 format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_mp4, test::FormatContextFixture )
{

    formatContext = transcode::util::openFormatContext(VIDEO_MP4);
}

/**
 * Test open flv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_flv, test::FormatContextFixture )
{

    formatContext = transcode::util::openFormatContext(VIDEO_FLV);
}

/**
 * Test open text format context.
 */
BOOST_FIXTURE_TEST_CASE( test_open_format_context_for_text_file, test::FormatContextFixture )
{

    formatContext = transcode::util::openFormatContext(TEXT_FILE);
}

/**
 * Test open empty format context.
 */
BOOST_AUTO_TEST_CASE( test_open_format_context_for_empty_file )
{

    BOOST_REQUIRE_THROW( transcode::util::openFormatContext(EMPTY_FILE),
            transcode::IOException );
}

/**
 * Test close avi format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_avi, test::AVIFormatContextFixture )
{

     transcode::util::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close mkv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_mkv, test::MKVFormatContextFixture )
{

     transcode::util::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close ogv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_ogv, test::OGVFormatContextFixture )
{

     transcode::util::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close mp4 format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_mp4, test::MP4FormatContextFixture )
{

     transcode::util::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close flv format context.
 */
BOOST_FIXTURE_TEST_CASE( test_close_format_context_for_flv, test::FLVFormatContextFixture )
{

     transcode::util::closeFormatContext(&formatContext);

     BOOST_REQUIRE_EQUAL( (AVFormatContext*) NULL, formatContext );
}

/**
 * Test close empty format context.
 */
BOOST_AUTO_TEST_CASE( test_close_empty_format_context )
{

    AVFormatContext *formatContext = new AVFormatContext();

    BOOST_REQUIRE_THROW( transcode::util::closeFormatContext(&formatContext),
               transcode::IllegalStateException );

    delete(formatContext);
}

/**
 * Test close NULL format context.
 */
BOOST_AUTO_TEST_CASE( test_close_null_format_context )
{

    AVFormatContext *formatContext = NULL;

    BOOST_REQUIRE_THROW( transcode::util::closeFormatContext(&formatContext),
               transcode::IllegalArgumentException );
}

/**
 * Test read packet from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_avi_file, test::AVIFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext) );
}

/**
 * Test read packet from a mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_mkv_file, test::MKVFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext) );
}

/**
 * Test read packet from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_ogv_file, test::OGVFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext) );
}

/**
 * Test read packet from a mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_mp4_file, test::MP4FormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext) );
}

/**
 * Test read packet from a flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_flv_file, test::FLVFormatContextFixture )
{

     BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext) );
}

/**
 * Test read packet from a text file.
 */
BOOST_FIXTURE_TEST_CASE( test_read_packet_from_text_file, test::TextFormatContextFixture )
{

    BOOST_REQUIRE( NULL != transcode::util::readNextPacket(formatContext) );
}

/**
 * Test read packet from an empty format context.
 */
BOOST_AUTO_TEST_CASE( test_read_packet_from_empty_format_context )
{

    AVFormatContext formatContext;
    formatContext.nb_streams = 0; // Only way to guaranty a packet read failure.

    BOOST_REQUIRE_THROW( transcode::util::readNextPacket(&formatContext),
            transcode::IllegalStateException );
}

/**
 * Test read packet from a NULL format context.
 */
BOOST_AUTO_TEST_CASE( test_read_packet_from_null_format_context )
{

    BOOST_REQUIRE_THROW( transcode::util::readNextPacket((AVFormatContext*) NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test find stream type from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_avi_file, test::AVIStreamFixture )
{

    BOOST_REQUIRE_EQUAL( DIVX_STREAM_ONE, transcode::util::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( DIVX_STREAM_TWO, transcode::util::findStreamType(streams[1]) );
}

/**
 * Test find stream type from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_mkv_file, test::AVIStreamFixture )
{

    BOOST_REQUIRE_EQUAL( MKV_STREAM_ONE, transcode::util::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( MKV_STREAM_TWO, transcode::util::findStreamType(streams[1]) );
}

/**
 * Test find stream type from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_ogv_file, test::OGVStreamFixture )
{

    BOOST_REQUIRE_EQUAL( OGV_STREAM_ONE, transcode::util::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_TWO, transcode::util::findStreamType(streams[1]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_THREE, transcode::util::findStreamType(streams[2]) );
}

/**
 * Test find stream type from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_mp4_file, test::MP4StreamFixture )
{

    BOOST_REQUIRE_EQUAL( MP4_STREAM_ONE, transcode::util::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( MP4_STREAM_TWO, transcode::util::findStreamType(streams[1]) );
}

/**
 * Test find stream type from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_stream_type_from_flv_file, test::FLVStreamFixture )
{

    BOOST_REQUIRE_EQUAL( FLV_STREAM_ONE, transcode::util::findStreamType(streams[0]) );
    BOOST_REQUIRE_EQUAL( FLV_STREAM_TWO, transcode::util::findStreamType(streams[1]) );
}

/**
 * Test find type for an empty stream.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_empty_stream )
{

    AVStream stream;
    stream.codec = NULL; // Only way for this test to be ok.

    BOOST_REQUIRE_THROW( transcode::util::findStreamType(&stream),
            transcode::IllegalArgumentException );
}

/**
 * Test find type for a null stream.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_null_stream )
{

    BOOST_REQUIRE_THROW( transcode::util::findStreamType(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test find codec type from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_avi_file, test::AVICodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( DIVX_STREAM_ONE, transcode::util::findCodecType(codecs[0]) );
    BOOST_REQUIRE_EQUAL( DIVX_STREAM_TWO, transcode::util::findCodecType(codecs[1]) );
}

/**
 * Test find codec type from an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_mkv_file, test::MKVCodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( MKV_STREAM_ONE, transcode::util::findCodecType(codecs[0]) );
    BOOST_REQUIRE_EQUAL( MKV_STREAM_TWO, transcode::util::findCodecType(codecs[1]) );
}

/**
 * Test find codec type from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_ogv_file, test::OGVCodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( OGV_STREAM_ONE, transcode::util::findCodecType(codecs[0]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_TWO, transcode::util::findCodecType(codecs[1]) );
    BOOST_REQUIRE_EQUAL( OGV_STREAM_THREE, transcode::util::findCodecType(codecs[2]) );
}

/**
 * Test find codec type from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_mp4_file, test::MP4CodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( MP4_STREAM_ONE, transcode::util::findCodecType(codecs[0]) );
    BOOST_REQUIRE_EQUAL( MP4_STREAM_TWO, transcode::util::findCodecType(codecs[1]) );
}

/**
 * Test find codec type from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_codec_type_from_flv_file, test::FLVCodecContextFixture )
{

    BOOST_REQUIRE_EQUAL( FLV_STREAM_ONE, transcode::util::findCodecType(codecs[0]) );
    BOOST_REQUIRE_EQUAL( FLV_STREAM_TWO, transcode::util::findCodecType(codecs[1]) );
}

/**
 * Test find type for an empty codec.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_empty_codec )
{

    AVCodecContext codec;

    BOOST_REQUIRE_THROW( transcode::util::findCodecType(&codec),
            transcode::IllegalStateException );
}

/**
 * Test find type for a null codec.
 */
BOOST_AUTO_TEST_CASE( test_find_type_for_null_codec )
{

    BOOST_REQUIRE_THROW( transcode::util::findCodecType(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test find packet type from an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_avi_file, test::AVIPacketFixture )
{

    BOOST_REQUIRE_EQUAL( DIVX_STREAM_ONE, transcode::util::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an mkvfile.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_mkv_file, test::MKVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( MKV_STREAM_ONE, transcode::util::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_ogv_file, test::OGVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( OGV_STREAM_THREE, transcode::util::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_mp4_file, test::MP4PacketFixture )
{

    BOOST_REQUIRE_EQUAL( MP4_STREAM_TWO, transcode::util::findPacketType(formatContext, packet) );
}

/**
 * Test find packet type from an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_flv_file, test::FLVPacketFixture )
{

    BOOST_REQUIRE_EQUAL( FLV_STREAM_TWO, transcode::util::findPacketType(formatContext, packet) );
}

/**
 * Test find type for a null format context.
 */
BOOST_FIXTURE_TEST_CASE( test_find_type_for_null_format_context, test::AVIPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::findPacketType(NULL, packet),
            transcode::IllegalArgumentException );
}

/**
 * Test find type for a null packet.
 */
BOOST_FIXTURE_TEST_CASE( test_find_type_for_null_packet, test::AVIFormatContextFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::findPacketType(formatContext, NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test open codecs for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_codecs_for_avi_file, test::AVICodecContextFixture )
{

    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[0]) );
    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[1]) );
}

/**
 * Test open codecs for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_codecs_for_mkv_file, test::MKVCodecContextFixture )
{

    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[0]) );
    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[1]) );
}

/**
 * Test open codecs for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_codecs_for_ogv_file, test::OGVCodecContextFixture )
{
// TODO: Subtitle codecs not yet supported.
//    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[0]) );
    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[1]) );
    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[2]) );
}

/**
 * Test open codecs for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_codecs_for_mp4_file, test::MP4CodecContextFixture )
{

    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[0]) );
    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[1]) );
}

/**
 * Test open codecs for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_open_codecs_for_flv_file, test::FLVCodecContextFixture )
{

    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[0]) );
    BOOST_REQUIRE( transcode::util::openCodecContext(codecs[1]) );
}

/**
 * Test open codec for a null codec.
 */
BOOST_AUTO_TEST_CASE( test_open_codec_for_null_codec )
{

    BOOST_REQUIRE_THROW( transcode::util::openCodecContext(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test close codecs for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_avi_file, test::AVIOpenedCodecContextFixture )
{

    transcode::util::closeCodecContext(&(codecs[0]));
    transcode::util::closeCodecContext(&(codecs[1]));
}

/**
 * Test close codecs for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_mkv_file, test::MKVOpenedCodecContextFixture )
{

    transcode::util::closeCodecContext(&(codecs[0]));
    transcode::util::closeCodecContext(&(codecs[1]));
}

/**
 * Test close codecs for an ogv file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_ogv_file, test::OGVOpenedCodecContextFixture )
{

    transcode::util::closeCodecContext(&(codecs[0]));
    transcode::util::closeCodecContext(&(codecs[1]));
    transcode::util::closeCodecContext(&(codecs[2]));
}

/**
 * Test close codecs for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_mp4_file, test::MP4OpenedCodecContextFixture )
{

    transcode::util::closeCodecContext(&(codecs[0]));
    transcode::util::closeCodecContext(&(codecs[1]));
}

/**
 * Test close codecs for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_close_codecs_for_flv_file, test::FLVOpenedCodecContextFixture )
{

    transcode::util::closeCodecContext(&(codecs[0]));
    transcode::util::closeCodecContext(&(codecs[1]));
}

/**
 * Test close codec for a null codec pointer.
 */
BOOST_AUTO_TEST_CASE( test_close_codec_for_null_codec_pointer )
{

    BOOST_REQUIRE_THROW( transcode::util::closeCodecContext(NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test close codec for a null codec.
 */
BOOST_AUTO_TEST_CASE( test_close_codec_for_null_codec )
{

    AVCodecContext *codecs[1];
    codecs[0] = NULL;

    BOOST_REQUIRE_THROW( transcode::util::closeCodecContext(codecs),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet for an avi file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_avi_file, test::AVIAudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::util::decodeAudioPacket(codecs[packet->stream_index], packet);

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet for an mkv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_mkv_file, test::MKVAudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::util::decodeAudioPacket(codecs[packet->stream_index], packet);

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

        frames = transcode::util::decodeAudioPacket(codecs[packet->stream_index], packet);

        // If the first audio packet doesn't decode read in the next one audio packet.
        if (0 < frames.size()) break;

        av_free_packet(packet);

        packet = readPacket(formatContext, AVMEDIA_TYPE_AUDIO);
    }

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet for an mp4 file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_mp4_file, test::MP4AudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::util::decodeAudioPacket(codecs[packet->stream_index], packet);

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet for an flv file.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_for_flv_file, test::FLVAudioPacketFixture )
{

    std::vector<AVFrame*> frames = transcode::util::decodeAudioPacket(codecs[packet->stream_index], packet);

    BOOST_REQUIRE_EQUAL( 1, frames.size() );
}

/**
 * Test decode audio packet with null codec.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_with_null_codec, test::AVIAudioPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::decodeAudioPacket(NULL, packet),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet with null packet.
 */
BOOST_FIXTURE_TEST_CASE( test_decode_audio_packet_with_null_packet, test::AVIAudioPacketFixture )
{

    BOOST_REQUIRE_THROW( transcode::util::decodeAudioPacket(codecs[packet->stream_index], NULL),
            transcode::IllegalArgumentException );
}

/**
 * Test decode audio packet with null codec and packet.
 */
BOOST_AUTO_TEST_CASE( test_decode_audio_packet_with_null_codec_and_packet )
{

    BOOST_REQUIRE_THROW( transcode::util::decodeAudioPacket(NULL, NULL),
            transcode::IllegalArgumentException );
}
