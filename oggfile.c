/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <iconv.h>
#include "vorbis/vorbisfile.h"
#include "oggfile.h"

static int      fill_comments(oggfile_t * oggfile);
static void     convert(char *in, char *out, size_t outlen);

static iconv_t  cd = NULL;

int
oggfile_create(oggfile_t * oggfile, char *filename)
{
        safe_strncpy(oggfile->filename, filename, FILENAMELEN);
        /*
         * TODO: There should be a better way (somewhere in function convert)
         * to make sure that the struct fields are correctly terminated
         */
        bzero(oggfile->artist, ARTISTLEN);
        bzero(oggfile->album, ALBUMLEN);
        bzero(oggfile->title, TITLELEN);
        if (fill_comments(oggfile))
                return 1;
        return 0;
}

static int
fill_comments(oggfile_t * oggfile)
{
        OggVorbis_File  ovf;
        vorbis_comment *ovc;
        int             i;
        char           *key;
        char           *value;

        if (ov_fopen(oggfile->filename, &ovf)) {
                warnx("could not open file: %s", oggfile->filename);
                return 1;
        }
        if ((ovc = ov_comment(&ovf, -1)) == NULL) {
                warnx("could not read comments for file: %s", oggfile->filename);
                return 1;
        }
        for (i = 0; i < ovc->comments; i++) {
                value = ovc->user_comments[i];
                key = strsep(&value, "=");
                if (!strcasecmp(key, "artist"))
                        convert(value, oggfile->artist, ARTISTLEN);
                else if (!strcasecmp(key, "album"))
                        convert(value, oggfile->album, ALBUMLEN);
                else if (!strcasecmp(key, "title"))
                        convert(value, oggfile->title, TITLELEN);
                if (oggfile->artist == NULL || oggfile->album == NULL || oggfile->title == NULL) {
                        warnx("insufficient comments for file: %s", oggfile->filename);
                        return 1;
                }
        }

        if (ov_clear(&ovf))
                warnx("could not close file: %s", oggfile->filename);

        return 0;
}

void
oggfile_setup()
{
        if (!cd)
                if ((cd = iconv_open(ENCODING, "UTF-8")) == (iconv_t) (-1))
                        errx(1, "could not open conversion descriptor");
}

void
oggfile_teardown()
{
        if (cd) {
                if (iconv_close(cd) == -1)
                        errx(1, "could not close conversion descriptor");
                cd = NULL;
        }
}

static void
convert(char *in, char *out, size_t outlen)
{
        char          **inp;
        char          **outp;
        size_t          inlen = strlen(in);

        inp = &in;
        outp = &out;

        if (iconv(cd, NULL, NULL, outp, &outlen) == (size_t) (-1))
                errx(1, "could not set initial conversion state");

        while (inlen > 0) {
                /*
                 * XXX: Don't know why the compiler complains about an
                 * incompatible pointer type for argument 2...
                 */
                if (iconv(cd, inp, &inlen, outp, &outlen) == (size_t) (-1))
                        errx(1, "string conversion failed");
        }

        return;
}
