/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <unistd.h>
#include <signal.h>
#include <err.h>
#include <sys/types.h>
#include "common.h"
#include "oggfile.h"
#include "player.h"

static pid_t    pid = (pid_t) (-1);

void
player_play(oggfile_t * oggfile)
{
        pid_t           lpid;

        player_stop();

        switch (lpid = fork()) {
        case (0):
                execl(OGG123, "ogg123", "-q", oggfile->filename, NULL);
        case (-1):
                err(1, "could not fork player");
        default:
                pid = lpid;
                return;
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
