/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <vorbis/vorbisfile.h>

#include "common.h"
#include "oggfile.h"

/* Prototypes */
static int      fill_comments(struct oggfile *oggfile);
static void     convert(char *in, char *out, size_t outlen);

/* Global variables */
static iconv_t  cd;

int
oggfile_create(struct oggfile *ogg, char *filename)
{
        assert(ogg != NULL);
        assert(filename != NULL);

        SAFE_STRNCPY(ogg->filename, filename, FILENAMELEN);
        ogg->artist[0] = '\0';
        ogg->album[0] = '\0';
        ogg->title[0] = '\0';
        if (fill_comments(ogg) != 0)
                return (1);

        return (0);
}

static int
fill_comments(struct oggfile *ogg)
{
        OggVorbis_File  ovf;
        vorbis_comment *ovc;
        int             i;
        char           *key;
        char           *value;

        assert(ogg != NULL);

        if (ov_fopen(ogg->filename, &ovf) != 0) {
                warnx("could not open file: %s", ogg->filename);
                return (1);
        }
        if ((ovc = ov_comment(&ovf, -1)) == NULL) {
                warnx("could not read comments for file: %s", ogg->filename);
                return (1);
        }
        for (i = 0; i < ovc->comments; i++) {
                value = ovc->user_comments[i];
                key = strsep(&value, "=");
                if (strcasecmp(key, "artist") == 0)
                        convert(value, ogg->artist, ARTISTLEN);
                else if (strcasecmp(key, "album") == 0)
                        convert(value, ogg->album, ALBUMLEN);
                else if (strcasecmp(key, "title") == 0)
                        convert(value, ogg->title, TITLELEN);
                if (ogg->artist == NULL || ogg->album == NULL || ogg->title == NULL) {
                        warnx("insufficient comments for file: %s", ogg->filename);
                        return (1);
                }
        }

        if (ov_clear(&ovf) != 0)
                warnx("could not close file: %s", ogg->filename);

        return (0);
}

void
oggfile_setup()
{
        if ((cd = iconv_open("char", "UTF-8")) == (iconv_t) (-1))
                errx(1, "could not open conversion descriptor");
}

void
oggfile_teardown()
{
        if (cd != (iconv_t) (-1))
                if (iconv_close(cd) == -1)
                        errx(1, "could not close conversion descriptor");
}

static void
convert(char *in, char *out, size_t outlen)
{
        char          **inp;
        char          **outp;
        size_t          inlen = strlen(in);

        assert(in != NULL);
        assert(out != NULL);
        assert(outlen > 0);

        inp = &in;
        outp = &out;

        if (iconv(cd, NULL, NULL, outp, &outlen) == (size_t) (-1))
                errx(1, "could not set initial conversion state");

        while (inlen > 0) {
                if (iconv(cd, (const char **)inp, &inlen, outp, &outlen) == (size_t) (-1))
                        errx(1, "string conversion failed");
        }

        *outp[0] = '\0';
}
