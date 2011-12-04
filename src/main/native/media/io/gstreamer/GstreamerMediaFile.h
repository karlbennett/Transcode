/*
 * GstreamerMediaFile.h
 *
 *  Created on: 6 Nov 2011
 *      Author: karl
 */

#ifndef GSTREAMERMEDIAFILE_H_
#define GSTREAMERMEDIAFILE_H_

#include <media/io/MediaFile.h>
#include <string>
#include <map>

namespace transcode {

/**
 * MediaFile implemented with the Gstreamer framework.
 */
class GstreamerMediaFile: public MediaFile {

	private:

		/**
		 * Initialise the a descriptions map container mime types mapped to a
		 * short description.
		 */
		static std::map<std::string, std::string> initDecriptions() {

			std::map<std::string, std::string> descriptions;

			descriptions["video/x-msvideo"] = "Audio Video Interleave (AVI)";
			descriptions["video/x-matroska"] = "Matroska Multimedia Container (MKV)";
			descriptions["application/ogg"] = "Ogg free open container format (Ogg)";
			descriptions["video/quicktime"] = "MPEG-4/QuickTime file format (MP4)";

			return descriptions;
		}

		/**
		 * A private static map to hold all the container descriptions.
		 */
		static std::map<std::string, std::string> CONTAINER_DESCRIPTIONS;

	public:
		/**
		 * Instantiate a GstreamerMediaFile and extract
		 * the properties of the media file at the provided path.
		 *
		 * @param string - the path to a media file.
		 *
		 * @throws MediaFileException if something went wrong
		 * 		   while extracting properties from the media file.
		 */
		GstreamerMediaFile(const std::string&);

		/**
		 * Return a description for the provided mime type otherwise return NULL;
		 *
		 * @param mimeType - the mime type that should have a related description.
		 *
		 * @return a description string for the provided mime type if one exists
		 * 		otherwise NULL;
		 */
		static std::string getDescription(const std::string& mimeType) {

			std::map<std::string, std::string>:: iterator it = CONTAINER_DESCRIPTIONS.find(mimeType);

			if (it != CONTAINER_DESCRIPTIONS.end()) {
				return it->second;
			}

			return NULL;
		}
};

} /* namespace transcode */

#endif /* GSTREAMERMEDIAFILE_H_ */
