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

    std::string mimeType_;

    MediaMetaData() :
            mimeType_("") {
    }

    /**
     * Construct a new MediaMetaData struct with the provided mimetype.
     *
     * @param mType - the mimeType for this MediaType struct.
     */
    MediaMetaData(const std::string& mimeType) :
            mimeType_(mimeType) {
    }
};

/**
 * Struct to hold all the common attributes for a subtitle media type.
 */
struct SubtitleMetaData: MediaMetaData {

    std::string language_;

    SubtitleMetaData() :
            MediaMetaData(), language_("") {
    }

    /**
     * Construct a new SubtitleDetail struct with the provided mimetype
     * and language.
     *
     * @param mType - the mimeType for this VideoDetails struct.
     * @param language - the language of the subtitle.
     */
    SubtitleMetaData(const std::string& mimeType, const std::string& language) :
            MediaMetaData(mimeType), language_(language) {
    }
};

/**
 * Struct to hold all the common attributes for an audio media type.
 */
struct AudioMetaData: MediaMetaData {

    std::string language_;
    int rate_;
    int channels_;

    AudioMetaData() :
            MediaMetaData(), language_(""), rate_(0), channels_(0) {
    }

    /**
     * Construct a new AudioDetails struct with the provided mimetype,
     * rate, and channel numbers.
     *
     * @param mType - the mimeType for this AudioDetails struct.
     * @param language - the language of the stream.
     * @param rate - the sample rate of the data, in samples (per channel) per second.
     * @param channels - the number of channels of audio data.
     */
    AudioMetaData(const std::string& mimeType, const std::string& language,
            const int& rate, const int& channels) :
            MediaMetaData(mimeType), language_(language), rate_(rate), channels_(
                    channels) {
    }
};

/**
 * Struct to hold all the common attributes for a video media type.
 */
struct VideoMetaData: MediaMetaData {

    int width_;
    int height_;
    int frameRate_;

    VideoMetaData() :
            MediaMetaData(), width_(0), height_(0), frameRate_(0) {
    }

    /**
     * Construct a new VideoDetails struct with the provided mimetype,
     * frame width, frame height, and frame rate.
     *
     * @param mType - the mimeType for this VideoDetails struct.
     * @param width - the width of the video image.
     * @param height - the height of the video image.
     * @param frameRate - the average frame rate in frames per second.
     * 					0 means a variable framerate.
     */
    VideoMetaData(const std::string& mimeType, const int& width,
            const int& height, const int& frameRate) :
            MediaMetaData(mimeType), width_(width), height_(height), frameRate_(
                    frameRate) {
    }
};

/**
 * Struct to hold all the common attributes for a container media type.
 */
struct ContainerMetaData: MediaMetaData {

    std::string description_;
    std::vector<SubtitleMetaData> subtitleDetails_;
    std::vector<AudioMetaData> audioDetails_;
    std::vector<VideoMetaData> videoDetails_;

    ContainerMetaData() :
            MediaMetaData(),
                    description_(""),
                    subtitleDetails_(std::vector<SubtitleMetaData>()),
                    audioDetails_(std::vector<AudioMetaData>()),
                    videoDetails_(std::vector<VideoMetaData>()) {
    }

    /**
     * Construct a new ContainerDetails struct with the provided mimetype,
     * description, audio, and video details.
     *
     * @param mType - the mimeType for this ContainerDetails struct.
     * @param description - a description of this container.
     * @param subtitleDetails - a list of subtitle details related to the
     * 						subtitle streams contained within this container.
     * @param audioDetails - a list of audio details related to the audio
     * 						streams contained within this container.
     * @param videoDetails - a list of video details related to the video
     * 						streams contained within this container.
     */
    ContainerMetaData(const std::string& mimeType,
            const std::string& description,
            const std::vector<SubtitleMetaData>& subtitleDetails,
            const std::vector<AudioMetaData>& audioDetails,
            const std::vector<VideoMetaData>& videoDetails) :
            MediaMetaData(mimeType),
                    description_(description),
                    subtitleDetails_(subtitleDetails),
                    audioDetails_(audioDetails),
                    videoDetails_(videoDetails) {
    }
};

/**
 * Struct to hold all the common attributes for a media file.
 */
struct MediaFileMetaData {

    ContainerMetaData container_;
    std::string path_;
    std::string name_;
    int size_;

    MediaFileMetaData() :
            container_(ContainerMetaData()), path_(""), name_(""), size_(0) {
    }

    /**
     * Construct a new MediaFileDetail struct with the provided container,
     * path, name, and size.
     *
     * @param container - the container of this media file.
     * @param path - a full path of this media file.
     * @param name - the name of this media file minus the path.
     * @param size - the size of this media file in bytes.
     */
    MediaFileMetaData(const ContainerMetaData& container, const std::string& path,
            const std::string& name, const int& size) :
            container_(container), path_(path), name_(name), size_(size) {
    }
};

} /* transcode */

#endif /* __META_DATA_HPP__ */
