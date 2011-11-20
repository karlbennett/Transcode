#ifndef __MEDIA_STREAM_H__
#define __MEDIA_STREAM_H__
#include <string>

namespace transcode {

class MediaStream {

    private:
        std::string mimeType;

    public:
        MediaStream(const std::string& mt): mimeType(mt) {}

        MediaStream(const MediaStream& mediaStream):
        	mimeType(mediaStream.mimeType) {}

        virtual ~MediaStream() {};

        std::string getMimeType() const { return mimeType; }
};

} /* namespace transcode */

#endif /* __MEDIA_STREAM_H__ */
