/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

struct plr_context;

struct plr_context *plr_context_open(char const *_ogg123, char const *_ogg123_options);
void            plr_context_close(struct plr_context *_ctx);
void            plr_play(struct plr_context *_ctx, char *_oggfile);
void            plr_stop(struct plr_context *_ctx);
