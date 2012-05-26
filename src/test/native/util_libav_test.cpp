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
BOOST_FIXTURE_TEST_CASE( test_find_packet_type_from_mkv_file, test::AVIPacketFixture )
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
