#ifndef __MEDIA_FILE_CPP__
#define __MEDIA_FILE_CPP__

#include <media/io/gstreamer/GstreamerMediaFile.h>
#include <media/io/exception/MediaFileException.h>
#include <media/MediaContainer.h>
#include <media/VideoStream.h>
#include <media/AudioStream.h>
#include <vector>
#include <string>
#include <glib.h>
#include <gst/gst.h>
#include <gst/playback/gstplay-enum.h>
#include <iostream>
#include <fstream>
#include <boost/xpressive/xpressive.hpp>


/**
 * Struct to hold all the attributes that need to be
 * accessed by the different call back functions.
 */
struct MediaFileStruct {

	transcode::MediaContainer mediaContainer;
	std::vector<transcode::AudioStream> audioStreams;
	std::vector<transcode::VideoStream> videoStreams;

	MediaFileStruct():
		mediaContainer("", ""),
		audioStreams(),
		videoStreams() {}
};

/**
 * Struct to hold all the attributes required for
 * error handling.
 */
struct ErrorStruct {

	GMainLoop *loop;
	std::string errorMessage;

	ErrorStruct(): loop(NULL), errorMessage("") {}
};


/**
 * Select the auto plug behaviour for the provided pad, always returns
 * GST_AUTOPLUG_SELECT_TRY.
 *
 * This call back is actually used to extract any relevant properties
 * from the provided Gstreamer caps.
 *
 * @param decoder - the decoder bin that called this function. Not used.
 * @param pad - the pad that was just created. Not used.
 * @param caps - the caps that were just created, the relevant properties
 * 				 are extracted from here.
 * @param factory - the factory that will be used to decode the stream
 * 					related to the pad. Not used.
 * @param mfStruct - the MediaFileStruct that will be populated by the
 * 					 properties extracted from the caps.
 */
GstAutoplugSelectResult selectAutoPlug(GstElement *decoder, GstPad *pad,
		GstCaps *caps, GstElementFactory *factory, MediaFileStruct *mfStruct) {

	using namespace transcode;

	// Get the structure of the pad from the caps, this holds
	// the information about the stream.
	GstStructure *str = gst_caps_get_structure(caps, 0);

	// Get the mime type of the stream.
	std::string mimeType = gst_structure_get_name(str);

	// If this is an audio stream then extract the related properties.
	if (g_strrstr(mimeType.c_str(), "audio")) {

		// If this is an audio stream then get it's rate and
		// channel number.
		if (g_strrstr(mimeType.c_str(), "audio")) {
			int rate, channels;

			gst_structure_get_int(str, "rate", &rate);
			gst_structure_get_int(str, "channels", &channels);

			mfStruct->audioStreams.push_back(
					AudioStream(mimeType, rate, channels));
		}

	// If this is a video then check if it is a container or actual
	// video stream.
	} else if (g_strrstr(mimeType.c_str(), "video")) {

		int width, height, framerate;

		// Try and retrieve the width and height from the stream.
		// If successful then this is a video stream so retrieve any
		// other properties.
		if (gst_structure_get_int(str, "width", &width)
				&& gst_structure_get_int(str, "height", &height)) {

			/*
			 We don't use framerate as a test to see if we
			 should populate the VideoStream object because
			 it isn't always present.
			 Also if it isn't present reset the value to zero
			 because otherwise it could be any value.
			 */
			if (!gst_structure_get_int(str, "framerate", &framerate))
				framerate = 0;

			mfStruct->videoStreams.push_back(
					VideoStream(mimeType, width, height, framerate));

		// If there is no width or height value then this must be a
		// container stream.
		} else {

			mfStruct->mediaContainer = MediaContainer(mimeType,
							GstreamerMediaFile::getDescription(mimeType));
		}
	}

	return GST_AUTOPLUG_SELECT_TRY;
}

/**
 * React to each message sent on a Gstreamer pipelines bus.
 *
 * If the message indicates the end of stream the GMainLoop will be stopped.
 * If the message is an error the GstreamerStruct will have it's error
 * message set and the GMainLoop will be stopped.
 *
 * @param bus - the Gstreamer pipelines bus
 * @param msg - the message that has just been sent on the pipelines bus.
 * @param bus - a pointer for a GstreamerStruct.
 *
 * @return TRUE, always.
 */
static gboolean messageSent(GstBus *bus, GstMessage *msg, gpointer data) {

	// Get the GstreamerStruct so that we can stop the GMainLoop or set
	// the error message.
	ErrorStruct *errorStruct = (ErrorStruct*) data;

	// See if we have seen an end of stream or error message.
	switch(GST_MESSAGE_TYPE (msg)) {

		// If we've seen an end of stream message just stop the GMainLoop.
		case GST_MESSAGE_EOS: {

			g_main_loop_quit (errorStruct->loop);
			break;
		}

		// If we've seen an error message then set the GstreamerStruct
		// error message string.
		case GST_MESSAGE_ERROR: {

			gchar *debug;
			GError *error;

			// Extract the error element.
			gst_message_parse_error (msg, &error, &debug);
			g_free (debug);

			// Set the error message string with the message string
			// from the error.
			errorStruct->errorMessage = error->message;
			g_error_free (error);

			g_main_loop_quit (errorStruct->loop);

			break;
		}
		default:
		break;
	}

	return TRUE;
}

/**
 * Terminate the provided GMainLoop. This function is a call back
 * that is used to terminate a running GMainLoop.
 *
 * @param lp - a pointer to the GMainLoop.
 *
 * @return FALSE so that this call back is only ever executed once.
 */
static gboolean exitLoop(gpointer loop) {

	g_main_loop_quit((GMainLoop *) loop);

	// This should only be called once.
	return FALSE;
}

/**
 * Stop the GMainLoop when there are no more demuxer pads to
 * process. This is a call back function that is registered to the
 * no-more-pads event for the demuxer element.
 *
 * @params demuxer - the demuxer element that has no more pads to be
 *                   processed.
 * @params loop    - the GMainLoop the main loop that is used to
 *                   pause the main thread till the pipeline
 *                   processing is complete.
 */
static void noMorePads(GstElement* demuxer, GMainLoop *loop) {

	// Stop the GMainLoop when there are no more pads to process.
	g_idle_add(exitLoop, loop);
}

/**
 * Initialise the map that holds the container descriptions.
 */
std::map<std::string, std::string> transcode::GstreamerMediaFile::CONTAINER_DESCRIPTIONS =
		transcode::GstreamerMediaFile::initDecriptions();


/**
 * Construct the a MediaFile object from the file at the provided
 * path.
 *
 * @param path - the path to the media file.
 */
transcode::GstreamerMediaFile::GstreamerMediaFile(const std::string& path):
		transcode::MediaFile() {

	// Initialise the Gstreamer framework.
	gst_init(NULL, NULL);

	/**
	 * A GMainLoop that is used to pause the main thread of this
	 * classes construction while the Gstreamer thread processes the
	 * related media file.
	 */
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);

	/*
	 Create references for:
	 - The pipeline that will be run to find the properties for
	 this file.
	 - The filesrc element that will be used to open the media
	 file.
	 - The decoder element that will be used to find the properties
	 of the media file.
	 */
	GstElement *pipeline, *filesrc, *decoder;

	// Instantiate the pipeline element with the name pipe.
	pipeline = gst_pipeline_new("pipe");

	// Instantiate the filesrc element with the name 'source'.
	filesrc = gst_element_factory_make("filesrc", "source");

	// Instantiate the decodedin2 element with the name 'decoder'.
	decoder = gst_element_factory_make("decodebin2", "decoder");

	// The struct that will hold the error message and a reference
	// to the loop so the message call back can terminate the
	// GMainLoop if there is an error.
	ErrorStruct errorStruct;

	// Set the reference to the GMainLoop.
	errorStruct.loop = loop;

	// Add the call back to handle any messages sent within the pipeline.
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
	gst_bus_add_watch(bus, messageSent, &errorStruct);
	gst_object_unref(bus);

	// Point the filesrc to the location provided in the path string.
	g_object_set(G_OBJECT (filesrc), "location", path.c_str(), NULL);

	// The MediaFileStruct that will hold the property values that will
	// be used to populate this class.
	MediaFileStruct mfStruct;

	/*
	 Set the call back function that will be called on the
	 'autoplug-select' event. This function will gradually populate the
	 MediaFileStruct each time it is called.
	 */
	g_signal_connect(decoder, "autoplug-select",
				G_CALLBACK (selectAutoPlug), &mfStruct);

	/*
	 Set the call back function that will be called on the
	 'no-more-pads' event. This function will terminate the GMainLoop.
	 */
	g_signal_connect(decoder, "no-more-pads", G_CALLBACK (noMorePads),
					loop);

	/*
	 Add the filesrc and decoder elements to the pipeline.
	 */
	gst_bin_add_many(GST_BIN (pipeline), filesrc, decoder, NULL);

	/*
	 Link the filesrc element to the decoder element. The media
	 file data will now be able to flow into the decoder element.
	 */
	gst_element_link(filesrc, decoder);

	// Start the Gstreamer pipeline.
	gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_PLAYING);

	// Start the GMainLoop to begin waiting until the decoder has
	// found all the media files properties.
	g_main_loop_run(loop);

	// Stop the pipeline and dereference it so that the memory
	// related to it and the elements contained within are freed.
	gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_NULL);
	gst_object_unref(GST_OBJECT (pipeline));

	// If the error message has been set then something went wrong
	// so throw an exception containing the error message.
	if (errorStruct.errorMessage.length() > 0) throw MediaFileException(errorStruct.errorMessage);

	// Populate this class.
	setFilePath(path);
	setMediaContainer(mfStruct.mediaContainer);
	setAudioStreams(mfStruct.audioStreams);
	setVideoStreams(mfStruct.videoStreams);
}

#endif /* __MEDIA_FILE_CPP__ */
