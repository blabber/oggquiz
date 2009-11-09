/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

enum {
        FILENAMELEN = 1024,
        OPTIONLEN = 64,
        ARTISTLEN = 128,
        ALBUMLEN = 128,
        TITLELEN = 128
};

#define SAFE_STRNCPY(dst, src, len)     do { \
                                        strncpy(dst, src, (len)-1); \
                                        dst[(len)-1] = '\0'; \
                                        } while (0)
