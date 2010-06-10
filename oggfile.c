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
#include <sysexits.h>
#include <vorbis/vorbisfile.h>

#include "oggfile.h"

struct ogg_context {
        iconv_t         cd;
};

static int      fill_comments(struct ogg_context *ctx, struct ogg_oggfile *ogg);
static void     do_iconv(struct ogg_context *ctx, char *in, char *out, size_t outlen);

int
ogg_oggfile_create(struct ogg_context *ctx, struct ogg_oggfile *ogg, char *filename)
{
        assert(ctx != NULL);
        assert(ogg != NULL);
        assert(filename != NULL);

        strncpy(ogg->filename, filename, sizeof(ogg->filename) - 1);
        ogg->filename[sizeof(ogg->filename) - 1] = '\0';

        if (fill_comments(ctx, ogg) != 0)
                return (1);

        return (0);
}

static int
fill_comments(struct ogg_context *ctx, struct ogg_oggfile *ogg)
{
        OggVorbis_File  ovf;
        vorbis_comment *ovc;
        char           *key, *value;
        int             i;

        assert(ctx != NULL);
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
                        do_iconv(ctx, value, ogg->artist, sizeof(ogg->artist));
                else if (strcasecmp(key, "album") == 0)
                        do_iconv(ctx, value, ogg->album, sizeof(ogg->album));
                else if (strcasecmp(key, "title") == 0)
                        do_iconv(ctx, value, ogg->title, sizeof(ogg->title));
                if (ogg->artist == NULL || ogg->album == NULL || ogg->title == NULL) {
                        warnx("insufficient comments for file: %s", ogg->filename);
                        return (1);
                }
        }

        if (ov_clear(&ovf) != 0)
                warnx("could not close file: %s", ogg->filename);

        return (0);
}

struct ogg_context *
ogg_context_open()
{
        struct ogg_context *ctx;
        iconv_t         cd;

        if ((cd = iconv_open("char", "UTF-8")) == (iconv_t) (-1))
                return (NULL);
        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                return (NULL);

        ctx->cd = cd;

        return (ctx);
}

int
ogg_context_close(struct ogg_context *ctx)
{
        assert(ctx != NULL);

        if (iconv_close(ctx->cd) == -1)
                return (1);
        free(ctx);

        return (0);
}

static void
do_iconv(struct ogg_context *ctx, char *in, char *out, size_t outlen)
{
        char          **inp;
        char          **outp;
        size_t          inlen = strlen(in);

        assert(ctx != NULL);
        assert(in != NULL);
        assert(out != NULL);
        assert(outlen > 0);

        inp = &in;
        outp = &out;

        if (iconv(ctx->cd, NULL, NULL, outp, &outlen) == (size_t) (-1))
                errx(EX_SOFTWARE, "could not set initial conversion state");

        while (inlen > 0) {
                if (iconv(ctx->cd, (const char **)inp, &inlen, outp, &outlen) == (size_t) (-1))
                        errx(EX_SOFTWARE, "string conversion failed");
        }

        *outp[0] = '\0';
}
