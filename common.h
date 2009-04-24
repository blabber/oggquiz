/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#ifndef _common_h_
#define _common_h_

#define FILENAMELEN     1024
#define ARTISTLEN       128
#define ALBUMLEN        128
#define TITLELEN        128

#define TIMEOUT         60
#define CHOICES         4
#define PLAYERS         4

#define ENCODING        "ISO-8859-15"
#define OGG123          "/usr/local/bin/ogg123"

#define safe_strncpy(dst, src, len)     { \
                                        strncpy(dst, src, (len)-1); \
                                        dst[(len)-1] = '\0'; \
                                        }

#define min(a, b)                       ((a) < (b)) ? a : b
#endif
