#ifndef __VIDEO_STREAM_H__
#define __VIDEO_STREAM_H__
#include <media/MediaStream.h>
#include <string>

namespace transcode {

class VideoStream: public MediaStream {

    private:
        int width;
        int height;
        int framerate;

    public:
        VideoStream(const std::string& mimeType, int w, int h, int fr):
        	MediaStream(mimeType), width(w), height(h), framerate(fr) {}

        VideoStream(const VideoStream& videaStream):
        	MediaStream(videaStream), width(videaStream.width),
        	height(videaStream.height),
        	framerate(videaStream.framerate) {}

        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getFramerate() const { return framerate; }
};

} /* namespace transcode */

#endif /* __VIDEO_STREAM_H__ */
