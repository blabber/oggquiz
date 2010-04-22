/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
#include <sys/types.h>

#include "player.h"

struct plr_context {
        char           *ogg123;
        char           *ogg123_options;
        pid_t           pid;
};

struct plr_context *
plr_context_open(char const *ogg123, char const *ogg123_options)
{
        struct plr_context *ctx;
        size_t          len;

        assert(ogg123 != NULL);

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(EX_SOFTWARE, "could not malloc plr_ctx");

        len = strlen(ogg123);
        if ((ctx->ogg123 = malloc(len + 1)) == NULL)
                err(EX_SOFTWARE, "could not malloc plr_ctx->ogg123");
        strncpy(ctx->ogg123, ogg123, len);
        ctx->ogg123[len] = '\0';

        len = strlen(ogg123_options);
        if (len > 0) {
                if ((ctx->ogg123_options = malloc(len + 1)) == NULL)
                        err(EX_SOFTWARE, "could not malloc plr_ctx->ogg123_options");
                strncpy(ctx->ogg123_options, ogg123_options, len);
                ctx->ogg123_options[len] = '\0';
        } else
                ctx->ogg123_options = NULL;

        ctx->pid = (pid_t) - 1;

        return (ctx);
}

void
plr_context_close(struct plr_context *ctx)
{
        assert(ctx != NULL);

        plr_stop(ctx);
        free(ctx->ogg123);
        free(ctx);
}

void
plr_play(struct plr_context *ctx, char *ogg)
{
        pid_t           lpid;

        assert(ctx != NULL);
        assert(ogg != NULL);

        plr_stop(ctx);

        switch (lpid = fork()) {
        case (0):
                execl(ctx->ogg123, ctx->ogg123, "-q", ogg, ctx->ogg123_options, NULL);
                err(EX_OSERR, "could not exec %s", ctx->ogg123);
        case (-1):
                err(EX_OSERR, "could not fork player");
        default:
                ctx->pid = lpid;
        }
}

void
plr_stop(struct plr_context *ctx)
{
        if (ctx->pid != (pid_t) (-1)) {
                if (kill(ctx->pid, SIGHUP))
                        err(EX_OSERR, "could not kill running player: %d", ctx->pid);
                ctx->pid = -1;
        }
}
