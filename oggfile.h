/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct ogg_oggfile {
        char            artist[128];
        char            album[128];
        char            title[128];
        char            filename[1024];
};

struct ogg_context;

int             ogg_context_close(struct ogg_context *_ctx);
struct ogg_context *ogg_context_open(void);
int             ogg_oggfile_create(struct ogg_context *_ctx, struct ogg_oggfile *_oggfile, char *_filename);
