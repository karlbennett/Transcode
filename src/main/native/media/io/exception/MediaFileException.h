/*
 * MediaFileException.h
 *
 *  Created on: 26 Nov 2011
 *      Author: karl
 */

#ifndef __MEDIA_FIL_EEXCEPTION_H___
#define __MEDIA_FIL_EEXCEPTION_H___

#include <string>

namespace transcode {

/**
 * Exception that is thrown when ever a MediaFile class can't
 * open the media file that it has been given the path to.
 */
class MediaFileException {

	private:
		std::string message;

	public:
		MediaFileException(std::string msg): message(msg) {}

		virtual ~MediaFileException() {}

		/**
		 * Get the message for this exception.
		 *
		 * @return the exceptions mesage.
		 */
		std::string getMessage() const { return message; }
};

} /* namespace transcode */

#endif /* __MEDIA_FIL_EEXCEPTION_H___ */
