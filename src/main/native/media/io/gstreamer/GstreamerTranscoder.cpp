/*
 * GstreamerTranscoder.cpp
 *
 *  Created on: 7 Nov 2011
 *      Author: karl
 */

#include <media/io/gstreamer/GstreamerTranscoder.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/pbutils/encoding-profile.h>
#include <gst/app/gstappsink.h>
#include <iostream>


/**
 * Call back function that is called when a pad is added to
 * the related element. An attempt is made to connect the new
 * pad to second provided element.
 *
 * @param decoder - the decoder element that has produced the
 *                  new pad.
 * @param pad     - the new pad that needs to be connected.
 * @param encoder - the encoder element that will have the
 *                  new pad connected to it.
 */
static void padAdded(GstElement * decoder, GstPad * pad, GstElement * encoder) {

	// Get the caps for the new pad, this will provide access
	// to information about the pad.
	GstCaps *caps = gst_pad_get_caps(pad);

	// Get the structure of the pad from the caps, this holds
	// the information about the pad.
	GstStructure *structure = gst_caps_get_structure(caps, 0);

	// Get the mimetype for this pad, this will tell if it is
	// a pad that we care about, e.g. audio or video.
	const gchar *mimeType = gst_structure_get_name(structure);

	// If this is an audio or video tag then we want to try and
	// connect it to the encoder element.
	if ((g_strrstr(mimeType, "video") || g_strrstr(mimeType, "audio"))) {

		// Create a reference to hold the compatible pad from
		// the encoder element that we will connect to the new decoder pad.
		GstPad *sinkpad;

		// Request a compatible pad from the encoder element.
		sinkpad = gst_element_get_compatible_pad(encoder, pad, NULL);

		// If the pad is link then don't try and link the new pad.
		if (gst_pad_is_linked(sinkpad)) {

			cout << "Pad already link: " << mimeType << endl;

		} else {

			// Otherwise try and link the new pad to the encoders compatible pad.
			if (G_UNLIKELY (gst_pad_link (pad, sinkpad) == GST_PAD_LINK_OK)) {

				cout << "Pad linked: " << mimeType << endl;

			} else {

				cout << "Error pad not linked: " << mimeType << endl;
			}
		}
	}
}

/**
 * Call back function that is called when the pipeline needs
 * to check if the current pad is going to be connected or if
 * it  should move on to the next pad.
 * Example: http://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-plugins/html/gst-plugins-base-plugins-decodebin2.html#GstDecodeBin2-autoplug-continue
 *
 * @param decoder    - the decoder element that produced the new pad.
 * @param checkedPad - the new pad that has just been created.
 * @param caps       - the caps for the new pad.
 * @param encoder    - the decoder element that will have the pad added to it.
 *
 * @return FALSE if the current pad is going to be used, otherwise TRUE.
 */
static gboolean autoplugContinue(GstElement *decoder, GstPad *checkedPad,
		GstCaps *caps, GstElement *encoder) {

	// A reference to hold the sink pad that was returned by the encoder.
	GstPad *sinkpad;

	// Try and see if the current pad can be connected to the encoder.
	g_signal_emit_by_name(encoder, "request-pad", caps, &sinkpad);

	// If it can't try the next pad.
	if (sinkpad == NULL)
		return TRUE;

	// Otherwise use this pad.
	return FALSE;
}

static gboolean errorSent(GstBus *bus, GstMessage *msg, gpointer data) {

	GMainLoop *loop = (GMainLoop*)data;

	gchar *debug;
	GError *error;

	// Extract the error element.
	gst_message_parse_error(msg, &error, &debug);
	g_free(debug);

	std::cout << "Error (" << GST_MESSAGE_TYPE_NAME(msg) << "): " << error->message << std::endl;

	g_error_free(error);
	g_main_loop_quit(loop);

	return TRUE;
}


/**
 * An Transcoder class implemented with the Gstreamer media framework.
 * This class can be used to transcode a media file into a raw binary
 * stream of a different media format. That is a media file with a
 * different container, video, and audio streams.
 *
 * @param in  - the input media file that is to be transcoded.
 * @param out - the output format that the media file is to be transcoded to.
 */
transcode::GstreamerTranscoder::GstreamerTranscoder(MediaFile in, MediaFile out) :
		Transcoder(in, out) {

	// Initialise the Gstreamer framework.
	gst_init(NULL, NULL);

	/**
	 * A GMainLoop that is used to pause the main thread of this
	 * classes construction while the Gstreamer thread processes the
	 * related media file.
	 */
	loop = g_main_loop_new(NULL, FALSE);

	/*
	 Create references for:
	 source - the filesrc element that will be used to open the media
	 file.

	 decoder - the decoder element that will decode the in media file.

	 encoder - the encoder element that will encode the out media file.
	 */
	GstElement *source, *decoder, *encoder;

	// Instantiate the pipeline element with the name 'transcoder'.
	pipeline = gst_pipeline_new("transcoder");

	// Instantiate the filesrc element with the name 'source'.
	source = gst_element_factory_make("filesrc", "source");

	// Instantiate the decoder element with the name 'decoder'.
	decoder = gst_element_factory_make("decodebin2", "decoder");

	// Instantiate the encoder element with the name 'encoder'.
	encoder = gst_element_factory_make("encodebin", "encoder");

	// Instantiate the sink element with the name 'sink'.
	sink = gst_element_factory_make("appsink", "sink");

	// Add the call back to handle any messages sent within the pipeline.
	GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
	gst_bus_add_signal_watch(bus);
	g_signal_connect(bus, "message::error", G_CALLBACK (errorSent), loop);
	gst_object_unref(bus);

	// Point the filesrc to the location of the input file.
	g_object_set(G_OBJECT (source), "location", in.getFilePath().c_str(), NULL);

	g_object_set(G_OBJECT (sink), "max-buffers", 128, NULL);

	// Create a reference for the container profile that will be used
	// to tell the encoder what format the output should be encoded to.
	GstEncodingContainerProfile *prof;

	// Create a reference for the caps that will be used to set the
	// properties for the container profile and it's internal video and
	// audio profiles.
	GstCaps *caps;

	// Get the container type for the output file.
	MediaContainer outPutContainer = out.getMediaContainer();

	// Create the caps for the container profile from the output file
	// containers mimetype.
	caps = gst_caps_from_string(outPutContainer.getMimeType().c_str());

	// Create the container profile using the previous caps and
	// description from the output media files container object.
	// Example: http://gstreamer.freedesktop.org/data/doc/gstreamer/head/gst-plugins-base-libs/html/gst-plugins-base-libs-encoding-profile.html
	prof = gst_encoding_container_profile_new(NULL,
			outPutContainer.getDescription().c_str(), caps, NULL);

	// Delete the previous caps because we no longer need them.
	gst_caps_unref(caps);

	// Get the first video stream from the output file.
	// TODO: Create profiles for all video streams.
	VideoStream outPutVideoStream = out.getVideoStream(0);

	// Create the caps for the video stream profile using the width,
	// height, and framerate of the output media files first video
	// stream object. Example: http://gstreamer.freedesktop.org/data/doc/gstreamer/head/manual/html/section-caps-api.html#section-caps-filter
	caps = gst_caps_new_simple(outPutVideoStream.getMimeType().c_str(), "width",
			G_TYPE_INT, outPutVideoStream.getWidth(), "height", G_TYPE_INT,
			outPutVideoStream.getHeight(), "framerate", GST_TYPE_FRACTION,
			outPutVideoStream.getFramerate(), 1, NULL);

	// Create the video stream profile using the width, height, and
	// framerate of the output media files first video stream object.
	gst_encoding_container_profile_add_profile(
			prof,
			(GstEncodingProfile*) gst_encoding_video_profile_new(caps, NULL,
					NULL, 0));

	// Delete the caps.
	gst_caps_unref(caps);

	// Get the first audio stream from the output file.
	// TODO: Create profiles for all audio streams.
	AudioStream outPutAudioStream = out.getAudioStream(0);

	// Create the caps for the audio stream profile using the sample rate and
	// channels of the output media files first audio stream object.
	caps = gst_caps_new_simple(outPutAudioStream.getMimeType().c_str(), "rate",
			G_TYPE_INT, outPutAudioStream.getSampleRate(), "channels",
			G_TYPE_INT, outPutAudioStream.getChannels(), NULL);

	// Create the audio stream profile using the sample rate and
	// channels of the output media files first audio stream object.
	gst_encoding_container_profile_add_profile(
			prof,
			(GstEncodingProfile*) gst_encoding_audio_profile_new(caps, NULL,
					NULL, 0));

	// Delete the caps.
	gst_caps_unref(caps);

	// Set the profile for the encoder to use.
	g_object_set(G_OBJECT (encoder), "profile", prof, NULL);

	// Add the source, decoder, encoder, and sink elements to the pipeline so
	// they are all ready to begin transcoding the input media file.
	gst_bin_add_many(GST_BIN (pipeline), source, decoder, encoder, sink, NULL);

	// The source element and decoder element can be linked automatically.
	gst_element_link(source, decoder);

	// As can the encoder and sink elements.
	gst_element_link(encoder, sink);

	// Register the call back for handling a new pad being added to the decoder
	// element.
	g_signal_connect(decoder, "pad-added", G_CALLBACK (padAdded), encoder);

	// Regester the call back for checking if a new pad is usable.
	g_signal_connect(decoder, "autoplug-continue",
			G_CALLBACK (autoplugContinue), encoder);

	// Start the Gstreamer pipeline.
	gst_element_set_state(pipeline, GST_STATE_PLAYING);

	g_main_loop_run(loop);

	// Pull the first buffer from the sink so we can ready it's data for
	// reading as well as find out the size of the first buffer.
	buffer = gst_app_sink_pull_buffer((GstAppSink*) sink);

	// Get the size of the first buffer.
	bufferSize = GST_BUFFER_SIZE(buffer);

	// Place the data from the first buffer into the bytes array.
	bytes = (char*) GST_BUFFER_DATA(buffer);

	// Set the number of bytes read to zero so the class is ready for reading.
	bytesRead = 0;
}

/**
 * The classes destructor.
 */
transcode::GstreamerTranscoder::~GstreamerTranscoder() {

	// Stop the pipeline and unreference it so that the memory
	// related to it and the elements contained within are freed.
	gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_NULL);
	gst_object_unref(GST_OBJECT (pipeline));
}

/**
 * Get a single bytes (char) worth of data from the transcoded stream.
 *
 * @param c - the byte (char) that is to be populated from the transcoded stream.
 */
void transcode::GstreamerTranscoder::get(char& c) {

	// If we have read all the bytes from the last allocated buffer then free
	// it and read in a new one.
	if (bytesRead >= bufferSize) {

		gst_buffer_unref(buffer); // Free the buffer and it's data.

		buffer = gst_app_sink_pull_buffer((GstAppSink*) sink); // Extract the next buffer.

		bufferSize = GST_BUFFER_SIZE(buffer); // Find the size of this new buffer.
		bytes = (char*) GST_BUFFER_DATA(buffer); // Extract it's data.
		bytesRead = 0; // Reset the bytes read.
	}

	c = bytes[bytesRead++];
}

/**
 * Get an array of bytes (char) from the transcoded stream.
 *
 * @param c - the array of bytes (char) that is to be populated from the
 *            transcoded stream.
 * @param n - the number of bytes (char) that are to be populated within
 *            the provided array.
 */
void transcode::GstreamerTranscoder::get(char *c, int n) {

	// Record to total number of bytes read so we know when to stop reading them.
	int totalBytesRead = 0;

	// Here we will store a single byte to be put into the provided byte array.
	char byte;

	// Read in bytes until we have read all that was requested.
	while (totalBytesRead < n) {

		get(byte); // Read a byte...

		c[totalBytesRead++] = byte; // ...and add it to the byte array.
	}
}
