/*
 * MediaUtils.hpp
 *
 *  Created on: 26 Feb 2012
 *      Author: karl
 */

#ifndef __META_DATA_HPP__
#define __META_DATA_HPP__

#include <string>
#include <vector>

namespace transcode {

/**
 * Struct to hold all the common attributes for a media type.
 */
struct MediaMetaData {

    std::string mimeType;

    MediaMetaData() :
            mimeType("") {
    }

    /**
     * Construct a new MediaMetaData struct with the provided mimetype.
     *
     * @param mType - the mimeType for this MediaType struct.
     */
    MediaMetaData(const std::string& mType) :
            mimeType(mType) {
    }
};

/**
 * Struct to hold all the common attributes for a subtitle media type.
 */
struct SubtitleMetaData: MediaMetaData {

    std::string language;

    SubtitleMetaData() :
            MediaMetaData(), language("") {
    }

    /**
     * Construct a new SubtitleDetail struct with the provided mimetype
     * and language.
     *
     * @param mimeType - the mimeType for this VideoDetails struct.
     * @param lang - the language of the subtitle.
     */
    SubtitleMetaData(const std::string& mimeType, const std::string& lang) :
            MediaMetaData(mimeType), language(lang) {
    }
};

/**
 * Struct to hold all the common attributes for an audio media type.
 */
struct AudioMetaData: MediaMetaData {

    std::string language;
    int rate;
    int channels;

    AudioMetaData() :
            MediaMetaData(), language(""), rate(0), channels(0) {
    }

    /**
     * Construct a new AudioDetails struct with the provided mimetype,
     * rate, and channel numbers.
     *
     * @param mimeType - the mimeType for this AudioDetails struct.
     * @param lang - the language of the stream.
     * @param r - the sample rate of the data, in samples (per channel) per second.
     * @param ch - the number of channels of audio data.
     */
    AudioMetaData(const std::string& mimeType, const std::string& lang,
            const int& r, const int& ch) :
            MediaMetaData(mimeType), language(lang), rate(r), channels(
                    ch) {
    }
};

/**
 * Struct to hold all the common attributes for a video media type.
 */
struct VideoMetaData: MediaMetaData {

    int width;
    int height;
    int frameRate;

    VideoMetaData() :
            MediaMetaData(), width(0), height(0), frameRate(0) {
    }

    /**
     * Construct a new VideoDetails struct with the provided mimetype,
     * frame width, frame height, and frame rate.
     *
     * @param mimeType - the mimeType for this VideoDetails struct.
     * @param w - the width of the video image.
     * @param h - the height of the video image.
     * @param fr - the average frame rate in frames per second.
     * 					0 means a variable framerate.
     */
    VideoMetaData(const std::string& mimeType, const int& w,
            const int& h, const int& fr) :
            MediaMetaData(mimeType), width(w), height(h), frameRate(
                    fr) {
    }
};

/**
 * Struct to hold all the common attributes for a container media type.
 */
struct ContainerMetaData: MediaMetaData {

    std::string description;
    std::vector<SubtitleMetaData> subtitleDetails;
    std::vector<AudioMetaData> audioDetails;
    std::vector<VideoMetaData> videoDetails;

    ContainerMetaData() :
            MediaMetaData(),
                    description(""),
                    subtitleDetails(std::vector<SubtitleMetaData>()),
                    audioDetails(std::vector<AudioMetaData>()),
                    videoDetails(std::vector<VideoMetaData>()) {
    }

    /**
     * Construct a new ContainerDetails struct with the provided mimetype,
     * description, audio, and video details.
     *
     * @param mimeType - the mimeType for this ContainerDetails struct.
     * @param desc - a description of this container.
     * @param subDets - a list of subtitle details related to the
     * 						subtitle streams contained within this container.
     * @param audioDets - a list of audio details related to the audio
     * 						streams contained within this container.
     * @param videoDets - a list of video details related to the video
     * 						streams contained within this container.
     */
    ContainerMetaData(const std::string& mimeType,
            const std::string& desc,
            const std::vector<SubtitleMetaData>& subDets,
            const std::vector<AudioMetaData>& audioDets,
            const std::vector<VideoMetaData>& videoDets) :
            MediaMetaData(mimeType),
                    description(desc),
                    subtitleDetails(subDets),
                    audioDetails(audioDets),
                    videoDetails(videoDets) {
    }
};

/**
 * Struct to hold all the common attributes for a media file.
 */
struct MediaFileMetaData {

    ContainerMetaData container;
    std::string path;
    std::string name;
    int size;

    MediaFileMetaData() :
            container(ContainerMetaData()), path(""), name(""), size(0) {
    }

    /**
     * Construct a new MediaFileDetail struct with the provided container,
     * path, name, and size.
     *
     * @param cont - the container of this media file.
     * @param p - a full path of this media file.
     * @param n - the name of this media file minus the path.
     * @param s - the size of this media file in bytes.
     */
    MediaFileMetaData(const ContainerMetaData& cont, const std::string& p,
            const std::string& n, const int& s) :
            container(cont), path(p), name(n), size(s) {
    }
};

} /* transcode */

#endif /* __META_DATA_HPP__ */
