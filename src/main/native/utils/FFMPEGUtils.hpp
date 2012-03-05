/*
 * FFMPEGUtils.hpp
 *
 *  Created on: 4 Mar 2012
 *      Author: karl
 */

#ifndef __FFMPEGUTILS_HPP__
#define __FFMPEGUTILS_HPP__

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include <utils/FileUtils.hpp>
#include <utils/MediaUtils.hpp>

#include <string>
#include <vector>
#include <map>

#include <tr1/functional>

namespace transcode {
namespace utils {
/**
 * Initialise the map that will be used to find the mime type for a
 * select few libav CodecID's.
 */
std::map<CodecID, std::string> initialiseCodecToMimeType();

/**
 * Initialise the map that will be used to find the mime type for a
 * select libav container names.
 */
std::map<std::string, std::string> initialiseNameToMimeType();

/**
 * The map that is used to look up mime types from libav CodecID's.
 */
const std::map<CodecID, std::string> CODEC_TO_MIMETYPE =
		initialiseCodecToMimeType();

/**
 * The map that is used to look up mime types from libav container names.
 */
const std::map<std::string, std::string> NAME_TO_MIMETYPE =
		initialiseNameToMimeType();

/**
 * Get the FFMPEG message for the provided error code.
 *
 * @param errorCode - the FFMPEG error code.
 *
 * @return the error message that relates to the provided error code.
 */
std::string ffmpegErrorMessage(int errorCode);

/**
 * Exception that is thrown if something goes wrong with the FFMPEG library.
 */
class FFMPEGException: public MediaUtilsException {

private:
	int errorCode;

public:
	/**
	 * Default constructor, set message to empty string and error code to 0.
	 */
	FFMPEGException() throw () :
			MediaUtilsException(), errorCode(0) {
	}

	/**
	 * Instantiate a new FFMPEGException with the provided error message and
	 * set the error code to 0.
	 *
	 * @param msg - the message for this exception.
	 */
	FFMPEGException(const std::string& msg) throw () :
			MediaUtilsException(msg), errorCode(0) {
	}

	/**
	 * Instantiate a new FFMPEGException with the provided error code and the
	 * message set automatically from the error code.
	 *
	 * @param ec - the FFMPEG error code for this exception.
	 */
	FFMPEGException(const int& ec) throw () :
			MediaUtilsException(ffmpegErrorMessage(ec)), errorCode(ec) {
	}

	/**
	 * Instantiate an FFMPEGException with the provided message and error code.
	 *
	 * @param msg - the message for this exception.
	 * @param ec - the FFMPEG error code for this exception.
	 */
	FFMPEGException(const std::string& msg, const int& ec) throw () :
			MediaUtilsException(msg), errorCode(ec) {
	}

	~FFMPEGException() throw () {
	}

	/**
	 * Get the error code for this exception.
	 *
	 * @return the error code for this exception.
	 */
	int getErrorCode() {
		return errorCode;
	}
};

/**
 * Initialise the FFMPEG api and create an AVFormatContext to be used to expose
 * information about the media file at the provided path.
 *
 * @param filePath - the path to the file that will have it's details inspected.
 *
 * @return an initialised AVFormatContext struct for the provided media file.
 */
AVFormatContext* initialiseFFMPEG(const std::string& filePath)
		throw (FFMPEGException);

/**
 * Extract the media details of the given type from the libav AVFormatContext struct
 * using the provided get details function.
 *
 * @param videoFile - the av format context that contains the audio details to extract.
 * @param mediaType - the type of media stream that should have it's codec inspected.
 * @param detailsCallback - the function to use to get the correct details out of the codec.
 *
 * @return a vector containing all the extracted details.
 */
template<typename T> std::vector<T> extractDetails(
		const AVFormatContext *videoFile, AVMediaType mediatype,
		std::tr1::function<T(const AVStream&)> detailsCallback) {

	std::vector<T> details;

	AVStream *stream;
	AVMediaType codecType;

	// Iterate through the streams searching for any audio streams.
	for (int i = 0; i < videoFile->nb_streams; i++) {

		stream = videoFile->streams[i];
		codecType = videoFile->streams[i]->codec->codec_type;

		// If we find an audio stream use its details to construct an AudioDetail
		// struct.
		if (codecType == mediatype) {

			details.push_back(detailsCallback(*stream));
		}
	}

	return details;
}

/**
 * Find the details of the requested type from the provided file using the
 * provided media type.
 *
 * @param fp - the path to the file that will have it's details inspected.
 * @param mediatype - the type of details that should be found.
 * @param detailsCallback - the function to use to get the correct details.
 *
 * @return a vector containing all the extracted details.
 */
template<typename T> std::vector<T> findDetails(std::string fp,
		AVMediaType mediatype,
		std::tr1::function<T(const AVStream&)> detailsCallback)
				throw (FFMPEGException) {

	try {

		(void) transcode::utils::checkFile(fp);

	} catch (std::exception& e) {

		throw FFMPEGException(e.what());
	}

	AVFormatContext *videoFile = initialiseFFMPEG(fp);

	return extractDetails<T>(videoFile, mediatype, detailsCallback);
}

/**
 * Extract the language from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant language.
 *
 * @return the name of the streams language if one has been set.
 */
std::string extractLanguage(const AVStream& stream);

/**
 * Extract the subtitle detail from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant subtitle details.
 *
 * @return a subtitle details struct populated from values within the provided codec.
 */
SubtitleDetail extractSubtitleDetail(const AVStream& stream);

/**
 * Extract the audio detail from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant audio details.
 *
 * @return an audio details struct populated from values within the provided codec.
 */
AudioDetail extractAudioDetail(const AVStream& stream);

/**
 * Extract the video detail from the provided libav AVStream.
 *
 * @param stream - the av stream that contains the relevant video details.
 *
 * @return a video details struct populated from values within the provided codec.
 */
VideoDetail extractVideoDetail(const AVStream& stream);

/**
 * Build a ContainerDetail struct out of the details in the provided
 * AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that contains the details to be
 * 			used to populate the ContainerDetail.
 *
 * @return a populated ContainerDetail struct.
 */
ContainerDetail buildContainerDetail(const AVFormatContext *videoFile);

/**
 * Close any codecs that are related to the provided AVFormatContext.
 *
 * @param videoFile - the AVFormatContext that will have all it's codecs closed.
 */
void closeCodecs(AVFormatContext *videoFile) throw (FFMPEGException);

} /* namespace utils */
} /* namespace transcode */

#endif /* __FFMPEGUTILS_HPP__ */
