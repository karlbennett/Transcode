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

// Forward decelerations to allow usage in other classes.
class MediaStream;

/**
 * Abstract class that represent a generic compressed frame of an any
 * type.
 */
class Frame {

private:
    std::vector<char> _data;

    unsigned long _presentationTimestamp;

    unsigned long _decompressionTimestamp;

    int _duration;

public:
    Frame() : _data(std::vector<char>()), _presentationTimestamp(0),
    _decompressionTimestamp(0), _duration(0) {}

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
    Frame(const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration) :
            _data(data),
                    _presentationTimestamp(presentationTimestamp),
                    _decompressionTimestamp(decompressionTimestamp),
                    _duration(duration) {
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
    virtual std::tr1::shared_ptr<E> decodeFrame();
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
    virtual std::tr1::shared_ptr<D> encodeFrame();
};

/**
 * An audio type that contains all the video related
 * attributes and methods.
 */
class AudioFrame: public Frame {

private:
    int _sampleNum;
    std::string _language;

public:
    AudioFrame() : Frame(), _sampleNum(0),
    _language(std::string()) {}

    /**
     * Instantiate an AudioFrame object with all the
     * required attributes.
     *
     * @param sampleNum - number of audio samples
     *      (per channel) described by this frame.
     * @param language - the language of the audio.
     */
    AudioFrame(const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration, const int& sampleNum,
            const std::string& language) :
            Frame(data, presentationTimestamp,
                    decompressionTimestamp, duration),
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
class VideoFrame: public Frame {

private:
    int _width;
    int _height;

public:
    VideoFrame() : Frame(), _width(0), _height(0) {}

    /**
     * Instantiate a VideoFrame object with all the required attributes.
     *
     * @param width - the width of the video frame.
     * @param heitgh - the height of the video frame.
     */
    VideoFrame(const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration, const int& width, const int& height) :
            Frame(data, presentationTimestamp,
                    decompressionTimestamp, duration),
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
