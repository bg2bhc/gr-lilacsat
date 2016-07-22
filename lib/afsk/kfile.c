/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2000, 2001, 2003 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 *
 * \brief Virtual KFile I/O interface.
 *
 * KFile is a simple, generic interface for file I/O.  It uses an
 * object-oriented model to supply a device-neutral interface to
 * communicate with drivers.
 *
 * This module contains only definitions, the instance structure
 * and the common API.
 * Each KFile subclass can override one or more methods of the interface,
 * and can extend the base KFile structure with its own private data.
 * For instance, a serial driver might implement the KFile interface by
 * declaring a context structure like this:
 *
 * \code
 * typedef struct Serial
 * {
 *      // base class instance
 *      KFile fd;
 *
 *      // private instance data
 *      FIFOBuffer txfifo, rxfifo;
 * } Serial;
 * \endcode
 *
 * You should also supply a macro for casting KFile to Serial:
 *
 * \code
 * INLINE Serial * SERIAL_CAST(KFile *fd)
 * {
 *		ASSERT(fd->_type == KFT_SERIAL);
 *		return (Serial *)fd;
 * }
 * \endcode
 *
 * Then you can implement as many interface functions as needed
 * and leave the rest to NULL.
 *
 * Example implementation of the close KFile method for Serial:
 *
 * \code
 * static int ser_kfile_close(struct KFile *fd)
 * {
 *		Serial *fds = SERIAL_CAST(fd);
 *      // [driver specific code here]
 *		return 0;
 * }
 * \endcode
 *
 * The SERIAL_CAST() macro helps ensure that the passed object is
 * really of type Serial.
 *
 * The KFile interface does not supply an open function: this is deliberate,
 * because in embedded systems each device has its own init parameters.
 * For the same reason, specific device settings like, for example,
 * the baudrate, are not part of interface and should be handled by the
 * driver-specific API.
 *
 * \version $Id: kfile.h 2975 2009-09-27 13:21:27Z batt $
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Francesco Sacchi <batt@develer.com>
 * \author Daniele Basile <asterix@develer.com>
 *
 * $WIZ$ module_name = "kfile"
 * $WIZ$ module_configuration = "bertos/cfg/cfg_kfile.h"
 * $WIZ$ module_depends = "timer", "formatwr"
 */


#include <string.h>
#include <stdarg.h>
#include "kfile.h"

/**
 * Interface functions for KFile access.
 * \note Remember to change following functions if KFile interface changes.
 * \{
 */
inline size_t kfile_read(struct KFile *fd, void *buf, size_t size)
{
//	ASSERT(fd->read);
	return fd->read(fd, buf, size);
}

inline size_t kfile_write(struct KFile *fd, const void *buf, size_t size)
{
//	ASSERT(fd->write);
	return fd->write(fd, buf, size);
}

inline KFile * kfile_reopen(struct KFile *fd)
{
//	ASSERT(fd->reopen);
	return fd->reopen(fd);
}

inline int kfile_close(struct KFile *fd)
{
//	ASSERT(fd->close);
	return fd->close(fd);
}

inline kfile_off_t kfile_seek(struct KFile *fd, kfile_off_t offset, KSeekMode whence)
{
//	ASSERT(fd->seek);
	return fd->seek(fd, offset, whence);
}

inline int kfile_flush(struct KFile *fd)
{
//	ASSERT(fd->flush);
	return fd->flush(fd);
}

inline int kfile_error(struct KFile *fd)
{
//	ASSERT(fd->error);
	return fd->error(fd);
}

inline void kfile_clearerr(struct KFile *fd)
{
//	ASSERT(fd->clearerr);
	fd->clearerr(fd);
}


/**
 * Generic putc() implementation using \a fd->write.
 */
int kfile_putc(int _c, struct KFile *fd)
{
	unsigned char c = (unsigned char)_c;

	if (kfile_write(fd, &c, sizeof(c)) == sizeof(c))
		return (int)((unsigned char)_c);
	else
		return EOF;
}

/**
 * Generic getc() implementation using \a fd->read.
 */
int kfile_getc(KFile *fd)
{
	unsigned char c;

	if (kfile_read(fd, &c, sizeof(c)) == sizeof(c))
		return (int)((unsigned char)c);
	else
		return EOF;
}


/**
 * Formatted write.
 */ /*
int kfile_printf(struct KFile *fd, const char *format, ...)
{
	va_list ap;
	int len;

	va_start(ap, format);
//	len = _formatted_write(format, (void (*)(char, void *))kfile_putc, fd, ap);
	va_end(ap);

	return len;
}
*/

