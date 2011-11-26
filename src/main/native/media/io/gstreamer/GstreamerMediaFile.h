/*
 * GstreamerMediaFile.h
 *
 *  Created on: 6 Nov 2011
 *      Author: karl
 */

#ifndef GSTREAMERMEDIAFILE_H_
#define GSTREAMERMEDIAFILE_H_

#include <media/io/MediaFile.h>

namespace transcode {

/**
 * MediaFile implemented with the Gstreamer framework.
 */
class GstreamerMediaFile: public MediaFile {

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
};

} /* namespace transcode */

#endif /* GSTREAMERMEDIAFILE_H_ */
