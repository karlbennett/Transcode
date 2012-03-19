/*
 * media.hpp
 *
 *  Created on: 18 Mar 2012
 *      Author: karl
 */

#ifndef __MEDIA_HPP__
#define __MEDIA_HPP__

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
class RawFrame;
class InputMediaStream;

/**
 * Abstract class that represent a generic compressed frame of an any
 * type.
 */
class Frame {

public:
    /**
     * Enum that represents the different types of frames.
     */
    enum FRAME_TYPE {
        SUBTITLE,
        AUDIO,
        VIDEO
    };

private:
    FRAME_TYPE _type;

    std::vector<char> _data;

    unsigned long _presentationTimestamp;

    unsigned long _decompressionTimestamp;

    int _duration;

    std::tr1::shared_ptr<InputMediaStream> _stream;

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
            const std::tr1::shared_ptr<InputMediaStream>& stream) :
            _type(type), _data(data),
                    _presentationTimestamp(presentationTimestamp),
                    _decompressionTimestamp(decompressionTimestamp),
                    _duration(duration),
                    _stream(stream) {
    }

    virtual ~Frame();

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
    std::tr1::shared_ptr<InputMediaStream> getStream() {
        return _stream;
    }

    /**
     * Decode this compressed frame into a RawFrame.
     *
     * @return a raw frame decoded from this frame.
     */
    virtual std::tr1::shared_ptr<RawFrame> decodeFrame() = 0;
};

/**
 * Abstract class that represent a generic decompressed frame of an any
 * type.
 */
class RawFrame {

public:
    virtual ~RawFrame();

    /**
     * Get the type of this frame.
     *
     * @return the frame type.
     */
    virtual Frame::FRAME_TYPE getType() = 0;

    /**
     * Get the data for this frame.
     *
     * @return the frame data.
     */
    virtual std::vector<char> getData() = 0;

    /**
     * Get the presentation time stamp of this frame.
     *
     * @return the frame presentation time stamp.
     */
    virtual unsigned long getPresentationTimestamp() = 0;

    /**
     * Get the decompression time stamp of this frame.
     *
     * @return the frame decompression time stamp.
     */
    virtual unsigned long getDecompressionTimestamp() = 0;

    /**
     * Get the duration of this frame.
     *
     * @return the frame duration.
     */
    virtual int getDuration() = 0;

    /**
     * Get the parent stream for this frame.
     *
     * NOTE: This stream should only be used for meta data, a call
     *       to the getFrame method on this stream will not get you the
     *       frame after this one.
     *
     * @return the frames parent stream.
     */
    virtual std::tr1::shared_ptr<InputMediaStream> getStream() = 0;

    /**
     * Encode this decompressed frame into a Frame.
     *
     * @return a frame encoded from this raw frame.
     */
    virtual std::tr1::shared_ptr<Frame> encodeFrame() = 0;
};

class AudioFrame: public Frame {

private:
    int _sampleNum;

public:
    /**
     * Instantiate an AudioFrame object with all the required attributes.
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
     * @param sampleNum - number of audio samples (per channel) described by
     *          this frame.
     */
    AudioFrame(const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<InputMediaStream>& stream,
            const int& sampleNum) :
            Frame(AUDIO, data,
                    presentationTimestamp, decompressionTimestamp,
                    duration, stream),
                    _sampleNum(sampleNum) {
    }

    /**
     * Get the sample rate for this AudioFrame.
     *
     * @return the audio frames sample rate.
     */
    int getSampleNum() {
        return _sampleNum;
    }

    std::tr1::shared_ptr<RawFrame> decodeFrame();
};

class RawAudioFrame: public RawFrame {

private:
    AudioFrame frame;

public:
    /**
     * Instantiate an RawAudioFrame object with all the required attributes.
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
     * @param sampleNum - number of audio samples (per channel) described by
     *          this frame.
     */
    RawAudioFrame(const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<InputMediaStream>& stream,
            const int& sampleNum) :
            frame(data, presentationTimestamp,
                    decompressionTimestamp, duration, stream,
                    sampleNum) {
    }

    Frame::FRAME_TYPE getType() {
        return frame.getType();
    }

    std::vector<char> getData() {
        return frame.getData();
    }

    unsigned long getPresentationTimestamp() {
        return frame.getPresentationTimestamp();
    }

    unsigned long getDecompressionTimestamp() {
        return frame.getDecompressionTimestamp();
    }

    int getDuration() {
        return frame.getDuration();
    }

    std::tr1::shared_ptr<InputMediaStream> getStream() {
        return frame.getStream();
    }

    int getSampleNum() {
        return frame.getSampleNum();
    }

    /**
     * Get the sample rate for this AudioFrame.
     *
     * @return the audio frames sample rate.
     */
    std::tr1::shared_ptr<Frame> encodeFrame();
};

class VideoFrame: public Frame {

private:
    int _width;
    int _height;

public:
    /**
     * Instantiate a VideoFrame object with all the required attributes.
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
     * @param width - the width of the video frame.
     * @param heitgh - the height of the video frame.
     */
    VideoFrame(const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<InputMediaStream>& stream,
            const int& width, const int& height) :
            Frame(VIDEO, data, presentationTimestamp,
                    decompressionTimestamp, duration, stream),
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

    std::tr1::shared_ptr<RawFrame> decodeFrame();
};

class RawVideoFrame: public RawFrame {

private:
    VideoFrame frame;

public:
    /**
     * Instantiate a RawVideoFrame object with all the required attributes.
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
     * @param width - the width of the video frame.
     * @param heitgh - the height of the video frame.
     */
    RawVideoFrame(const std::vector<char>& data,
            const unsigned long& presentationTimestamp,
            const unsigned long& decompressionTimestamp,
            const int& duration,
            const std::tr1::shared_ptr<InputMediaStream>& stream,
            const int& width, const int& height) :
            frame(data, presentationTimestamp,
                    decompressionTimestamp, duration, stream,
                    width, height) {
    }

    Frame::FRAME_TYPE getType() {
        return frame.getType();
    }

    std::vector<char> getData() {
        return frame.getData();
    }

    unsigned long getPresentationTimestamp() {
        return frame.getPresentationTimestamp();
    }

    unsigned long getDecompressionTimestamp() {
        return frame.getDecompressionTimestamp();
    }

    int getDuration() {
        return frame.getDuration();
    }

    std::tr1::shared_ptr<InputMediaStream> getStream() {
        return frame.getStream();
    }

    /**
     * Get the width of this VideoFrame.
     *
     * @return the video frame width.
     */
    int getWidth() {
        return frame.getWidth();
    }

    /**
     * Get the height of this VideoFrame.
     *
     * @return the video frame height.
     */
    int getHeight() {
        return frame.getHeight();
    }

    std::tr1::shared_ptr<Frame> encodeFrame();
};

/**
 * A media stream that provides access to the binary
 * streams within a media file.
 */
class InputMediaStream {

public:
    int getIndex();

    std::tr1::shared_ptr<Frame> getFrame();
};

} /* namespace transcode */

#endif /* __MEDIA_HPP__ */
