/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include "common.h"

#ifndef _oggfile_c_
#define _oggfile_c_

typedef struct {
        char            artist[ARTISTLEN];
        char            album[ALBUMLEN];
        char            title[TITLELEN];
        char            filename[FILENAMELEN];

} oggfile_t;

int             oggfile_create(oggfile_t *oggfile, char *filename);
void            oggfile_setup();
void            oggfile_teardown();

#endif
