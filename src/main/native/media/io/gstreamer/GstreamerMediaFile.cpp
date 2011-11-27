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
#include <iostream>
#include <fstream>
#include <boost/xpressive/xpressive.hpp>


/**
 * Struct to hold all the attributes that need to be
 * accessed by the different call back functions.
 */
struct GstreamerStruct {

	transcode::MediaContainer *mediaContainer;
	std::vector<transcode::AudioStream> *audioStreams;
	std::vector<transcode::VideoStream> *videoStreams;
	GMainLoop *loop;
	GstElement *pipeline;
	std::string errorMessage;
};


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
 * Retrieve the required properties from the added pad.
 * This function is a callback that is regestered to the demuxers
 * 'pad-added' event.
 *
 * @param demuxer  - a pointer to the demuxer that has had the pad
 *                   added.
 * @param pad      - a pointer to the pad that has been added.
 * @param args[2]  - An array that contains pointers to:
 *                   args[0] = (std::vector<AudioStream>*) the
 *                   MediaFile objects audioStreams attribute.
 *                   args[1] = (std::vector<VideoStream>*) the
 *                   MediaFile objects videoStreams attribute.
 */
static void padAdded(GstElement *demuxer, GstPad *pad,
		GstreamerStruct& gstStruct) {

	using namespace transcode;

	// Get the caps for the new pad, this will provide access
	// to information about the stream related to this pad..
	GstCaps *caps = gst_pad_get_caps(pad);

	// Get the structure of the pad from the caps, this holds
	// the information about the stream.
	GstStructure *str = gst_caps_get_structure(caps, 0);

	// Get the mime type of the stream.
	std::string mimeType = gst_structure_get_name(str);

	// If this is an audio or video stream then find it's
	// properties and connect it to the fakesink.
	if (g_strrstr(mimeType.c_str(), "audio")
			|| g_strrstr(mimeType.c_str(), "video")) {

		// If this is an audio stream then get it's rate and
		// channel number.
		if (g_strrstr(mimeType.c_str(), "audio")) {
			int rate, channels;

			gst_structure_get_int(str, "rate", &rate);
			gst_structure_get_int(str, "channels", &channels);

			gstStruct.audioStreams->push_back(AudioStream(mimeType, rate, channels));
		}

		// If this is a video stream then get it's width, height,
		// and framerate.
		if (g_strrstr(mimeType.c_str(), "video")) {
			int width, height, framerate;

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

				gstStruct.videoStreams->push_back(
						VideoStream(mimeType, width, height, framerate));
			}
		}
	}

	// Dereference the caps so they can be cleaned up nicely.
	gst_caps_unref(caps);
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
 * Find the correct demuxer element from the mime type in the
 * provided caps. This function is a call back that is regestered to
 * the typefinders 'have-type' event.
 *
 * @param typefind    - the typefind element that this call back has
 *                      been regestered to.
 * @param probability - the probability that the provided caps are
 *                      correct.
 * @param caps        - the caps for the media files container type.
 * @param args[3]     - An array that contains pointers to:
 *                      args[0] = (GMainLoop*) the main loop that is
 *                                used to pause the main thread till
 *                                the pipeline processing is
 *                                complete.
 *                      args[1] = (GstElement**) the demuxer element
 *                                that will be used to demux
 *                                themedia file to find the
 *                                properties for it's
 *                                audio and video streams.
 *                      args[2] = (MediaContainer*) this MediFile
 *                                objects MediaContainer attribute.
 */
static void typeFound(GstElement *typefind, guint probability, GstCaps *caps,
		GstreamerStruct& gstStruct) {

	using namespace transcode;

	// Retrieve the mimetype for the media files container.
	std::string containerMimeType = gst_caps_to_string(caps);
	std::string containerDescription;

	/*
	 Use the boost::xpressive name space within this function so
	 we don't have to prepend the name space before every boost
	 component.
	 */
	using namespace boost::xpressive;

	// Create a static regular expression to capture the importent
	// part of the containers mime type.
	sregex rex = bos >> (s1 = +_w >> '/' >> +(_w | '.' | '-')) >> *_;

	// Instantiate a capture component.
	smatch capture;

	// Match the containers mime type with the regular expresion
	// and if it matches replace it with the captured string.
	if (regex_match(containerMimeType, capture, rex)) {

		containerMimeType = capture[1].str();
	}

	// This will point to the demuxer that will be added to the pipeline.
	GstElement *demuxer = NULL;

	/*
	 Create the correct demuxer element for the mediafiles
	 container mimetype and set the description to be used
	 my the MediaContainer object.
	 */
	if (std::string("video/x-msvideo").compare(containerMimeType) == 0) {

		demuxer = gst_element_factory_make("avidemux", "demuxer");

		containerDescription = "Audio Video Interleave (AVI)";

	} else if (std::string("video/x-matroska").compare(containerMimeType)
			== 0) {

		demuxer = gst_element_factory_make("matroskademux", "demuxer");

		containerDescription = "Matroska Multimedia Container (MKV)";

	} else if (std::string("application/ogg").compare(containerMimeType) == 0) {

		demuxer = gst_element_factory_make("oggdemux", "demuxer");

		containerDescription = "Ogg free open container format (Ogg)";

	} else if (std::string("video/quicktime").compare(containerMimeType) == 0) {

		demuxer = gst_element_factory_make("qtdemux", "demuxer");

		containerDescription = "MPEG-4/QuickTime file format (MP4)";
	}

	// If we have managed to acquire a demuxer element then start decoding the media file.
	if (demuxer != NULL) {

		// Set the value for the MediaFile objects mediaContainer
		// attribute.
		*gstStruct.mediaContainer = MediaContainer(containerMimeType,
				containerDescription);

		// Add the demuxer element to the pipeline and set it's state to ready so
		// that it will process the data that is sent to it.
		gst_bin_add(GST_BIN (gstStruct.pipeline), demuxer);
		gst_element_set_state(demuxer, GST_STATE_READY);

		/*
		 Register the call back for the demuxers 'pad-added' event. It's in
		 this call back that the MediaFile objects audioStreams and videoStreams
		 attributes are populated.
		 */
		g_signal_connect(demuxer, "pad-added", G_CALLBACK (padAdded),
				&gstStruct);

		// Register the call back for the demuxers 'no-more-pads' events, it is
		// in this call back that the GMainLoop will be terminated.
		g_signal_connect(demuxer, "no-more-pads", G_CALLBACK (noMorePads),
				gstStruct.loop);

		// Retrieve the typefinders source pad so that it can be linked to the
		// demuxers sink pad.
		GstPad *src = gst_element_get_static_pad(typefind, "src");

		// Retrieve the demuxers sink pad so that it can be linked to the
		// typefinders source pad.
		GstPad *sink = gst_element_get_static_pad(demuxer, "sink");

		// Try and link the typefinders srouce pad to the demuxers sink pad
		// so that the media file data can flow into the demuxer.
		if (G_UNLIKELY (gst_pad_link (src, sink) != GST_PAD_LINK_OK)) {

			gstStruct.errorMessage = "Linking pads failed.";
		}
	} else {

		gstStruct.errorMessage = "Could not find a related demuxer for the file.";

		g_main_loop_quit (gstStruct.loop);
	}
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
	GstreamerStruct *gstStruct = (GstreamerStruct*) data;

	// See if we have seen an end of stream or error message.
	switch(GST_MESSAGE_TYPE (msg)) {

		// If we've seen an end of stream message just stop the GMainLoop.
		case GST_MESSAGE_EOS: {

			g_main_loop_quit (gstStruct->loop);
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
			gstStruct->errorMessage = error->message;
			g_error_free (error);

			g_main_loop_quit (gstStruct->loop);

			break;
		}
		default:
		break;
	}

	return TRUE;
}



/**
 * Construct the a MediaFile object from the file at the provided
 * path.
 *
 * @param uri - the path to the media file.
 */
transcode::GstreamerMediaFile::GstreamerMediaFile(const std::string& uri):
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
	 - The typefind element that will be used to find the container
	 type for the file.
	 - The demuxer that will be dynamically selected by the
	 mimetype that is found by the type find plugin.
	 - The fake sink that will collect and throw away any data
	 that makes it to the end of the pipeline.
	 */
	GstElement *pipeline, *filesrc, *typefind;

	// Instantiate the pipeline element.
	pipeline = gst_pipeline_new("pipe");

	// Instantiate the filesrc element with the name 'source'.
	filesrc = gst_element_factory_make("filesrc", "source");

	// Instantiate the typefind element with the name 'typefinder'.
	typefind = gst_element_factory_make("typefind", "typefinder");

	// Point the filesrc to the location provided in the uri
	// string.
	g_object_set(G_OBJECT (filesrc), "location", uri.c_str(), NULL);

	/*
	 Create the variables that will be placed within thes
	 GstreamerStruct. They have been created in the scope
	 of the constructor so that they aren't destroyed till
	 the end of the constructor.
	*/
	MediaContainer mediaContainer("", "");
	std::vector<AudioStream> audioStreams;
	std::vector<VideoStream> videoStreams;

	// Populate the GstreamerStruct so that the call back functions
	// can access all the elements they require.
	GstreamerStruct gstStruct;

	gstStruct.mediaContainer = &mediaContainer;
	gstStruct.audioStreams = &audioStreams;
	gstStruct.videoStreams = &videoStreams;
	gstStruct.loop = loop;
	gstStruct.pipeline = pipeline;

	// Add a call back to handle any messaged sent within the pipeline.
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
	gst_bus_add_watch(bus, messageSent, &gstStruct);
	gst_object_unref(bus);

	/*
	 Set the call back function that will be called on the
	 'have-type' event within the typefind element.
	 The demuxer pointer is passed by reference as the call back
	 functions data so that it can be pointed to a new demuxer
	 element.
	 */
	g_signal_connect(typefind, "have-type", G_CALLBACK (typeFound),
			&gstStruct);

	/*
	 Add the filesrc and typefind elements to the pipeline. A demuxer
	 element will be added after the typefind element has found the
	 file containers mimetype.
	 */
	gst_bin_add_many(GST_BIN (pipeline), filesrc, typefind, NULL);

	/*
	 Link the filesrc element to the typefind element. The media
	 file data will now be able to flow into the typefind element
	 and it will then be able to find the mimetype of the media
	 files container.
	 */
	gst_element_link(filesrc, typefind);

	// Start the Gstreamer pipeline.
	gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_PLAYING);

	// Start the global main loop to begin waiting until the
	// typefinder has found the the media files container mimetype.
	g_main_loop_run(loop);

	// Stop the pipeline and unreference it so that the memory
	// related to it and the elements contained within are freed.
	gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_NULL);
	gst_object_unref(GST_OBJECT (pipeline));

	// If the error message has been set then something went wrong
	// so throw an exception containing the error message.
	if (gstStruct.errorMessage.length() > 0) throw MediaFileException(gstStruct.errorMessage);

	setFileUri(uri);
	setMediaContainer(mediaContainer);
	setAudioStreams(audioStreams);
	setVideoStreams(videoStreams);
}

#endif /* __MEDIA_FILE_CPP__ */
