/*
 * MediaUtils.cpp
 *
 *  Created on: 26 Feb 2012
 *      Author: karl
 */

#ifndef __MEDIA_UTILS_CPP__
#define __MEDIA_UTILS_CPP__

#include <utils/MediaUtils.hpp>

namespace transcode {

MediaFileDetail findMediaFileDetails(const std::string& filePath) {

	std::vector<AudioDetail> audioDetails;
	std::vector<VideoDetail> videoDetails;

	ContainerDetail container("", "", audioDetails, videoDetails);

	return MediaFileDetail(container, "", "", 0);
}

}

#endif /* __MEDIA_UTILS_CPP__ */
