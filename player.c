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
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "oggfile.h"
#include "oggquiz.h"
#include "player.h"

/* Global variables */
static pid_t    pid = (pid_t) (-1);

void
player_play(struct oggfile *oggfile, struct options *opts)
{
        pid_t           lpid;

        assert(oggfile !=NULL);
        assert(opts != NULL);

        player_stop();

        switch (lpid = fork()) {
        case (0):
                execl(opts->ogg123, "ogg123", "-q", oggfile->filename, NULL);
                err(1, "could not exec %s", opts->ogg123);
        case (-1):
                err(1, "could not fork player");
        default:
                pid = lpid;
        }
}

void
player_stop()
{
        if (pid != (pid_t) (-1)) {
                if (kill(pid, SIGHUP))
                        err(1, "could not kill running player");
                pid = -1;
        }
}
