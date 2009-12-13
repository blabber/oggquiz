/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <err.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <time.h>

#include "options.h"
#include "oggfile.h"
#include "player.h"
#include "ui.h"

#define MIN(a, b)                       ((a) < (b)) ? (a) : (b)

enum {
        FILENAMELEN = 1024
};

static void     avoid_zombie_processes(void);
static int      new_turn(struct ogg_oggfile *oggfiles, struct plr_context *plr_ctx, struct ui_context *ui_ctx, struct opts_options *opts);

int
main(int argc, char **argv)
{
        char            filename[FILENAMELEN];
        char           *newline;
        int             oggfileno;
        struct ogg_oggfile *oggfiles;
        struct opts_options opts;
        struct ogg_context *ogg_ctx;
        struct plr_context *plr_ctx;
        struct ui_context *ui_ctx;

        assert(argc >= 0);
        assert(argv != NULL);

        if (setlocale(LC_ALL, "") == NULL)
                warnx("could not set locale");

        opts_parse_options(&opts, argc, argv);
        /*
         * After this point the opts_options structure is considered read
         * only!
         */

        if ((ogg_ctx = ogg_context_open()) == NULL)
                errx(EX_SOFTWARE, "could not open oggfile context");
        if ((plr_ctx = plr_context_open(opts.ogg123, opts.ogg123_options)) == NULL)
                errx(EX_SOFTWARE, "could not open player context");
        if ((ui_ctx = ui_context_open()) == NULL)
                errx(EX_SOFTWARE, "could not open ui context");
        if ((oggfiles = malloc(opts.choices * sizeof(struct ogg_oggfile))) == NULL)
                err(EX_SOFTWARE, "could not malloc oggfiles");
        avoid_zombie_processes();

        oggfileno = 0;
        while (fgets(filename, FILENAMELEN, stdin) != NULL) {
                if ((newline = strchr(filename, '\n')) != NULL)
                        newline[0] = '\0';

                if (ogg_oggfile_create(ogg_ctx, &oggfiles[oggfileno], filename) == 0)
                        oggfileno++;

                if (oggfileno == opts.choices) {
                        oggfileno = 0;
                        if (new_turn(oggfiles, plr_ctx, ui_ctx, &opts))
                                break;
                }
        }

        plr_stop(plr_ctx);
        plr_context_close(plr_ctx);
        ui_context_close(ui_ctx);
        if (ogg_context_close(ogg_ctx) != 0)
                warn("could not close oggfile context");

        return (0);
}

static int
new_turn(struct ogg_oggfile *oggfiles, struct plr_context *plr_ctx, struct ui_context *ui_ctx, struct opts_options *opts)
{
        static int      first_invocation = 0;
        static struct ui_model model;
        int             correct;
        int             i;
        char            guess;
        time_t          start;

        assert(oggfiles != NULL);
        assert(opts != NULL);

        if (!first_invocation) {
                first_invocation = 1;
                srand(time(NULL));
                if ((model.scores = malloc(opts->players * sizeof(int))) == NULL)
                        err(EX_SOFTWARE, "could not malloc scores");
                if ((model.oggfiles = malloc(opts->choices * sizeof(struct ui_oggfile))) == NULL)
                        err(EX_SOFTWARE, "could not malloc oggfiles");
                for (i = 0; i < opts->players; i++) {
                        model.scores[i] = 0;
                }
                model.turn = 0;
                model.players = opts->players;
                model.choices = opts->choices;
        }
        model.turn++;
        model.current_player = ((model.turn - 1) % model.players);
        for (i = 0; i < model.choices; i++) {
                model.oggfiles[i].title = oggfiles[i].title;
                model.oggfiles[i].artist = oggfiles[i].artist;
                model.oggfiles[i].album = oggfiles[i].album;
        }
        correct = rand() % model.choices;
        model.correct = &model.oggfiles[correct];
        ui_display_quiz(ui_ctx, &model);
        plr_play(plr_ctx, oggfiles[correct].filename);
        start = time(NULL);
        do {
                guess = ui_get_key();
                if (guess == 'q')
                        return (1);
        } while (guess < '1' || guess > '0' + opts->choices);
        model.guess = &model.oggfiles[guess - '1'];
        if (model.guess == model.correct)
                model.score_delta = MIN(opts->time, time(NULL) - start);
        else
                model.score_delta = opts->time;
        model.scores[model.current_player] += model.score_delta;
        ui_display_result(ui_ctx, &model);
        if (ui_get_key() == 'q')
                return (1);

        return (0);
}

static void
avoid_zombie_processes()
{
        struct sigaction sa;

        sa.sa_handler = SIG_IGN;
        sa.sa_flags = 0;
        sigemptyset(&(sa.sa_mask));
        sigaddset(&(sa.sa_mask), SIGPIPE);

        sigaction(SIGPIPE, &sa, NULL);
}
