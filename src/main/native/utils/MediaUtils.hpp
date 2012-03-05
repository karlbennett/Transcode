/*
 * MediaUtils.hpp
 *
 *  Created on: 26 Feb 2012
 *      Author: karl
 */

#ifndef __MEDIA_UTILS_HPP__
#define __MEDIA_UTILS_HPP__

#include <string>
#include <exception>
#include <vector>
#include <boost/rational.hpp>

namespace transcode {

/**
 * An exception that is thrown from within the MediaUtils functions.
 */
class MediaUtilsException: public std::exception {

private:
	std::string message;

public:
	MediaUtilsException() throw () :
			exception(), message("") {
	}

	/**
	 * Instantiate a MediaUtilsException object with the provided message.
	 */
	MediaUtilsException(std::string msg) throw () :
			exception(), message(msg) {
	}

	~MediaUtilsException() throw () {
	}

	const char* what() const throw () {
		return message.c_str();
	}
};

/**
 * Struct to hold all the common attributes for a media type.
 */
struct MediaDetail {

	std::string mimeType;

	MediaDetail() :
			mimeType("") {
	}

	/**
	 * Construct a new MediaDetails struct with the provided mimetype.
	 *
	 * @param mType - the mimeType for this MediaType struct.
	 */
	MediaDetail(const std::string& mType) :
			mimeType(mType) {
	}
};

/**
 * Struct to hold all the common attributes for a subtitle media type.
 */
struct SubtitleDetail: MediaDetail {

	std::string language;

	SubtitleDetail() :
			MediaDetail(), language("") {
	}

	/**
	 * Construct a new SubtitleDetail struct with the provided mimetype
	 * and language.
	 *
	 * @param mType - the mimeType for this VideoDetails struct.
	 * @param language - the language of the subtitle.
	 */
	SubtitleDetail(const std::string& mType, const std::string& lang) :
			MediaDetail(mType), language(lang) {
	}
};

/**
 * Struct to hold all the common attributes for an audio media type.
 */
struct AudioDetail: SubtitleDetail {

	int rate;
	int channels;

	AudioDetail() :
			SubtitleDetail(), rate(0), channels(0) {
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
	AudioDetail(const std::string& mType, const std::string& language,
			const int& rt, const int& ch) :
			SubtitleDetail(mType, language), rate(rt), channels(ch) {
	}
};

/**
 * Struct to hold all the common attributes for a video media type.
 */
struct VideoDetail: MediaDetail {

	int width;
	int height;
	int frameRate;

	VideoDetail() :
			MediaDetail(), width(0), height(0), frameRate(0) {
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
	VideoDetail(const std::string& mType, const int& w, const int& h,
			const int& fr) :
			MediaDetail(mType), width(w), height(h), frameRate(fr) {
	}
};

/**
 * Struct to hold all the common attributes for a container media type.
 */
struct ContainerDetail: MediaDetail {

	std::string description;
	std::vector<SubtitleDetail> subtitleDetails;
	std::vector<AudioDetail> audioDetails;
	std::vector<VideoDetail> videoDetails;

	ContainerDetail() :
			MediaDetail(), description(""), subtitleDetails(
					std::vector<SubtitleDetail>()), audioDetails(
					std::vector<AudioDetail>()), videoDetails(
					std::vector<VideoDetail>()) {
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
	ContainerDetail(const std::string& mType, const std::string& desc,
			const std::vector<SubtitleDetail>& sdts,
			const std::vector<AudioDetail>& adts,
			const std::vector<VideoDetail>& vdts) :
			MediaDetail(mType), description(desc), subtitleDetails(sdts), audioDetails(
					adts), videoDetails(vdts) {
	}
};

/**
 * Struct to hold all the common attributes for a media file.
 */
struct MediaFileDetail {

	ContainerDetail container;
	std::string path;
	std::string name;
	int size;

	MediaFileDetail() :
			container(ContainerDetail()), path(""), name(""), size(0) {
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
	MediaFileDetail(const ContainerDetail& cntr, const std::string& p,
			const std::string& n, const int& s) :
			container(cntr), path(p), name(n), size(s) {
	}
};

/**
 * Find the subtitle details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the subtitle details for the provided media file if possible.
 */
std::vector<SubtitleDetail> findSubtitleDetails(const std::string& filePath)
		throw (MediaUtilsException);

/**
 * Find the audio details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the audio details for the provided media file if possible.
 */
std::vector<AudioDetail> findAudioDetails(const std::string& filePath)
		throw (MediaUtilsException);

/**
 * Find the video details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the video details for the provided media file if possible.
 */
std::vector<VideoDetail> findVideoDetails(const std::string& filePath)
		throw (MediaUtilsException);

/**
 * Find the container details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the container details for the provided media file if possible.
 */
ContainerDetail findContainerDetails(const std::string& filePath)
		throw (MediaUtilsException);

/**
 * Find the details for the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return the details for the provided media file if possible.
 */
MediaFileDetail findMediaFileDetails(const std::string& filePath)
		throw (MediaUtilsException);

}

#endif /* __MEDIA_UTILS_HPP__ */
