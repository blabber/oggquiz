/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

/**
 * Required includes:
 * common.h
 */

struct oggfile {
        char            artist[ARTISTLEN];
        char            album[ALBUMLEN];
        char            title[TITLELEN];
        char            filename[FILENAMELEN];
};

int             oggfile_create(struct oggfile *_oggfile, char *_filename);
void            oggfile_setup(void);
void            oggfile_teardown(void);
