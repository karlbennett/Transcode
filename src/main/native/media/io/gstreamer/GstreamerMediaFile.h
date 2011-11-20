/*
 * GstreamerMediaFile.h
 *
 *  Created on: 6 Nov 2011
 *      Author: karl
 */

#ifndef GSTREAMERMEDIAFILE_H_
#define GSTREAMERMEDIAFILE_H_

#include <media/io/MediaFile.h>
#include <glib.h>
#include <gst/gst.h>

namespace transcode {

class GstreamerMediaFile: public MediaFile {

	public:
		GstreamerMediaFile(const std::string&);
};

} /* namespace transcode */

#endif /* GSTREAMERMEDIAFILE_H_ */
