/*
 * media.hpp
 *
 *  Created on: 18 Mar 2012
 *      Author: karl
 */

#ifndef __MEDIA_HPP__
#define __MEDIA_HPP__

#include <metadata.hpp>

#include <error.hpp>

#include <vector>
#include <boost/tr1/memory.hpp>

/**
 * @file media.hpp
 *
 * The main trancode API, a lot of the functions, classes and documentation within
 * this file have been heavily influenced by the libav (http://libav.org) project.
 */

/**
 * Transcode namespace, all the top level transcode functions and classes are in
 * this namespace. So this namespace holds officially public API.
 */
namespace transcode {

/**
 * Enum that represents the different types of frames.
 */
enum FRAME_TYPE {
    SUBTITLE,
    AUDIO,
    VIDEO
};

// Forward decelerations to allow usage in other classes.
class MediaStream;

/**
 * Abstract class that represent a generic compressed frame of an any
 * type.
 */
class Frame {

private:
    FRAME_TYPE _type;

    std::vector<char> _data;

    unsigned long _presentationTimestamp;

    unsigned long _decompressionTimestamp;

    int _duration;

    std::tr1::shared_ptr<MediaStream> _stream;

public:
    /**
     * Instantiate a Frame object with all the required attributes.
     *
     * @param type - the type of the frame.
     * @param data - the bytes that make up the frame.
     * @param presentationTimestamp - the time in seconds at which
     *          the decompressed packet will be presented to the user.
     * @param decompressionTimestamp - the time in seconds at which
     *          the packet is decompressed.
     * @param duration - the duration of this packet in seconds, 0 if unknown.
     * @param stream - the stream that this frame was extracted from.
     *          NOTE: This stream should only be used for meta data, a call
     *          to the getFrame method on this stream will not get you the
     *          frame after this one.
     */
    Frame(const FRAME_TYPE& type,
            const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<MediaStream>& stream) :
            _type(type), _data(data),
                    _presentationTimestamp(presentationTimestamp),
                    _decompressionTimestamp(decompressionTimestamp),
                    _duration(duration),
                    _stream(stream) {
    }

    /**
     * Get the type of this frame.
     *
     * @return the frame type.
     */
    FRAME_TYPE getType() {
        return _type;
    }

    /**
     * Get the data for this frame.
     *
     * @return the frame data.
     */
    std::vector<char> getData() {
        return _data;
    }

    /**
     * Get the presentation time stamp of this frame.
     *
     * @return the frame presentation time stamp.
     */
    unsigned long getPresentationTimestamp() {
        return _presentationTimestamp;
    }

    /**
     * Get the decompression time stamp of this frame.
     *
     * @return the frame decompression time stamp.
     */
    unsigned long getDecompressionTimestamp() {
        return _decompressionTimestamp;
    }

    /**
     * Get the duration of this frame.
     *
     * @return the frame duration.
     */
    int getDuration() {
        return _duration;
    }

    /**
     * Get the parent stream for this frame.
     *
     * NOTE: This stream should only be used for meta data, a call
     *       to the getFrame method on this stream will not get you the
     *       frame after this one.
     *
     * @return the frames parent stream.
     */
    std::tr1::shared_ptr<MediaStream> getStream() {
        return _stream;
    }
};

/**
 * Represents a frame that can be decoded.
 */
template<class E> class Decodable {

public:
    virtual ~Decodable();

    /**
     * Decode this compressed frame into a RawFrame.
     *
     * @return a raw frame decoded from this frame.
     */
    std::tr1::shared_ptr<E> decodeFrame();
};

/**
 * Represents a frame that can be encoded.
 */
template<class D> class Encodable {

public:
    virtual ~Encodable();

    /**
     * Encode this frame.
     *
     * @return a frame encoded from this raw frame.
     */
    std::tr1::shared_ptr<D> encodeFrame();
};

/**
 * An audio type that contains all the video related
 * attributes and methods.
 */
class Audio {

private:
    int _sampleNum;
    std::string _language;

public:
    /**
     * Instantiate an AudioFrame object with all the
     * required attributes.
     *
     * @param sampleNum - number of audio samples
     *      (per channel) described by this frame.
     * @param language - the language of the audio.
     */
    Audio(const int& sampleNum, const std::string& language) :
            _sampleNum(sampleNum), _language(language) {
    }

    /**
     * Get the sample rate for this AudioFrame.
     *
     * @return the audio frames sample rate.
     */
    int getSampleNum() {
        return _sampleNum;
    }
};

/**
 * A video type that contains all the video related
 * attributes and methods.
 */
class Video {

private:
    int _width;
    int _height;

public:
    /**
     * Instantiate a VideoFrame object with all the required attributes.
     *
     * @param width - the width of the video frame.
     * @param heitgh - the height of the video frame.
     */
    Video(const int& width, const int& height) :
            _width(width), _height(height) {
    }

    /**
     * Get the width of this VideoFrame.
     *
     * @return the video frame width.
     */
    int getWidth() {
        return _width;
    }

    /**
     * Get the height of this VideoFrame.
     *
     * @return the video frame height.
     */
    int getHeight() {
        return _height;
    }
};

class DecodedFrame;

/**
 * An encoded or compressed Frame, that has the ability to
 * be decoded.
 */
class EncodedFrame: public Frame, Decodable<DecodedFrame> {

public:
    EncodedFrame(const FRAME_TYPE& type,
            const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<MediaStream>& stream) :
            Frame(type, data, presentationTimestamp,
                    decompressionTimestamp, duration, stream) {
    }

    virtual ~EncodedFrame();

    virtual std::tr1::shared_ptr<DecodedFrame> decodeFrame();
};

/**
 * An decoded or compressed Frame, that has the ability to
 * be encoded.
 */
class DecodedFrame: public Frame, Decodable<EncodedFrame> {

public:
    DecodedFrame(const FRAME_TYPE& type,
            const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<MediaStream>& stream) :
            Frame(type, data, presentationTimestamp,
                    decompressionTimestamp, duration, stream) {
    }

    virtual ~DecodedFrame();

    virtual std::tr1::shared_ptr<EncodedFrame> encodeFrame();
};

class DecodedAudioFrame;

/**
 * An encoded or compressed audio frame.
 */
class EncodedAudioFrame: public DecodedFrame, Audio {

public:
    EncodedAudioFrame(
            const FRAME_TYPE& type,
            const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<MediaStream>& stream,
            const int& sampleNum, const std::string& language) :
            DecodedFrame(type, data, presentationTimestamp,
                    decompressionTimestamp, duration, stream),
                    Audio(sampleNum, language) {
    }

    std::tr1::shared_ptr<EncodedFrame> decodeFrame();

    std::tr1::shared_ptr<DecodedAudioFrame> decodeAudioFrame();
};

/**
 * A decoded or decompressed audio frame.
 */
class DecodedAudioFrame: public EncodedFrame, Audio {

public:
    DecodedAudioFrame(
            const FRAME_TYPE& type,
            const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<MediaStream>& stream,
            const int& sampleNum, const std::string& language) :
            EncodedFrame(type, data, presentationTimestamp,
                    decompressionTimestamp, duration, stream),
                    Audio(sampleNum, language) {
    }

    std::tr1::shared_ptr<EncodedFrame> encodeFrame();

    std::tr1::shared_ptr<EncodedAudioFrame> encodeAudioFrame();
};

class DecodedVideoFrame;

/**
 * An encoded or compressed video frame.
 */
class EncodedVideoFrame: public DecodedFrame, Video {

public:
    EncodedVideoFrame(
            const FRAME_TYPE& type,
            const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<MediaStream>& stream,
            const int& width, const int& height) :
            DecodedFrame(type, data, presentationTimestamp,
                    decompressionTimestamp, duration, stream),
                    Video(width, height) {
    }

    std::tr1::shared_ptr<EncodedFrame> decodeFrame();

    std::tr1::shared_ptr<DecodedVideoFrame> decodeVideoFrame();
};

/**
 * A decoded or decompressed video frame.
 */
class DecodedVideoFrame: public EncodedFrame, Video {

public:
    DecodedVideoFrame(
            const FRAME_TYPE& type,
            const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<MediaStream>& stream,
            const int& width, const int& height) :
            EncodedFrame(type, data, presentationTimestamp,
                    decompressionTimestamp, duration, stream),
                    Video(width, height) {
    }

    std::tr1::shared_ptr<EncodedFrame> encodeFrame();

    std::tr1::shared_ptr<EncodedVideoFrame> encodeVideoFrame();
};

class MediaStream {

private:
    int _index;

public:
    MediaStream() :
            _index(0) {
    }
    ;

    MediaStream(const int& index) :
            _index(index) {
    }
    ;

    int getIndex() {
        return _index;
    }
};

} /* namespace transcode */

#endif /* __MEDIA_HPP__ */
