#ifndef __MEDIA_CONTAINER_H__
#define __MEDIA_CONTAINER_H__
#include <media/MediaStream.h>
#include <string>

namespace transcode {

class MediaContainer: public MediaStream {

    private:
		std::string description;

    public:
        MediaContainer(const std::string& mimeType, const std::string& description):
        	MediaStream(mimeType), description(description) {}

        MediaContainer(const MediaContainer& mediacontainer):
        	MediaStream(mediacontainer), description(mediacontainer.description) {}

        std::string getDescription() const { return description; }
};

} /* namespace transcode */

#endif /* __MEDIA_CONTAINER_H__ */
