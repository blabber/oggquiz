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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "options.h"
#include "oggfile.h"
#include "oggquiz.h"
#include "player.h"
#include "ui.h"

#define MIN(a, b)                       ((a) < (b)) ? a : b

enum {
        CHOICES = 4,
        TIMEOUT = 60
};

/* Prototypes */
static int      new_turn(struct oggfile *oggfiles, struct options *opts);
static void     parse_options(struct options *opts, int argc, char **argv);
static void     usage(void);

int
main(int argc, char **argv)
{
        char            filename[FILENAMELEN];
        char           *newline;
        int             oggfileno;
        struct oggfile  oggfiles[CHOICES];
        struct options  opts;

        assert(argc >= 0);
        assert(argv != NULL);

        if (setlocale(LC_ALL, "") == NULL)
                warnx("could not set locale");

        parse_options(&opts, argc, argv);
        /*
         * After this point the global options structure is considered read
         * only!
         */

        oggfile_setup();
        ui_setup();

        oggfileno = 0;
        while (fgets(filename, FILENAMELEN, stdin) != NULL) {
                if ((newline = strchr(filename, '\n')) != NULL)
                        newline[0] = '\0';

                if (oggfile_create(&oggfiles[oggfileno], filename) == 0)
                        oggfileno++;

                if (oggfileno == opts.choices) {
                        oggfileno = 0;
                        if (new_turn(oggfiles, &opts))
                                break;
                }
        }

        ui_teardown();
        oggfile_teardown();
        player_stop();

        return (0);
}

static int
new_turn(struct oggfile *oggfiles, struct options *opts)
{
        static int      first_invocation = 0;
        static struct ui_model model;
        int             correct;
        char            guess;
        time_t          start;

        assert(oggfiles != NULL);
        assert(opts != NULL);

        if (!first_invocation) {
                first_invocation = 1;
                srand(time(NULL));
                /* abusing the correct variable as a temporary variable */
                for (correct = 0; correct < opts->players; correct++) {
                        model.scores[correct] = 0;
                        model.turn = 0;
                }
        }
        model.turn++;
        model.current_player = ((model.turn - 1) % opts->players);
        model.oggfiles = oggfiles;
        correct = rand() % opts->choices;
        model.correct = &oggfiles[correct];
        ui_display_quiz(&model, opts);
        player_play(model.correct, opts);
        start = time(NULL);
        do {
                guess = ui_get_answer();
                if (guess == 'q')
                        return (1);
        } while (guess < '1' || guess > '0' + opts->choices);
        model.guess = &oggfiles[guess - '1'];
        if (model.guess == model.correct)
                model.scores[model.current_player] += MIN(opts->time, time(NULL) - start);
        else
                model.scores[model.current_player] += opts->time;
        ui_display_result(&model, opts);
        ui_pause();

        return (0);
}

static void
parse_options(struct options *old_opts, int argc, char **argv)
{
        struct opts_options opts;

        assert(old_opts != NULL);
        assert(argc >= 0);
        assert(argv != NULL);

        opts_parse_options(&opts, argc, argv);

        old_opts->time = opts.time;
        old_opts->choices = opts.choices;
        old_opts->players = opts.players;
        old_opts->ogg123 = opts.ogg123;
}
