/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <err.h>
#include "oggquiz.h"
#include "common.h"
#include "oggfile.h"
#include "player.h"
#include "ui.h"

/* prototypes */
static void     init_options();
static int      new_turn(oggfile_t * oggfiles);
static void     parse_options(int argc, char **argv);

options_t       options;

int
main(int argc, char **argv)
{
        char            filename[FILENAMELEN];
        char           *newline;
        int             oggfileno = 0;
        oggfile_t       oggfiles[CHOICES];

        init_options();
        parse_options(argc, argv);

        oggfile_setup();
        ui_setup();

        while (fgets(filename, FILENAMELEN, stdin) != NULL) {
                if ((newline = strchr(filename, '\n')) != NULL)
                        newline[0] = '\0';

                if (!oggfile_create(&oggfiles[oggfileno], filename))
                        oggfileno++;

                if (oggfileno == options.choices) {
                        oggfileno = 0;
                        if (new_turn(oggfiles))
                                break;
                }
        }

        ui_teardown();
        oggfile_teardown();
        player_stop();

        return 0;
}

static int
new_turn(oggfile_t * oggfiles)
{
        static int      first_invocation = 0;
        static ui_model_t model;
        int             correct;
        char            guess;
        time_t          start;

        if (!first_invocation) {
                first_invocation = 1;
                srand(time(NULL));
                /* abusing the correct variable as a temporary variable */
                for (correct = 0; correct < options.choices; correct++) {
                        model.scores[correct] = 0;
                        model.turn = 0;
                }
        }
        model.turn++;
        model.current_player = ((model.turn - 1) % options.players);
        model.oggfiles = oggfiles;
        correct = rand() % options.choices;
        model.correct = &oggfiles[correct];
        ui_display_quiz(&model);
        player_play(model.correct);
        start = time(NULL);
        guess = ui_get_answer();
        if (guess == 'q')
                return 1;
        model.guess = &oggfiles[guess - '1'];
        if (model.guess == model.correct)
                model.scores[model.current_player] += MIN(options.time, time(NULL) - start);
        else
                model.scores[model.current_player] += options.time;
        ui_display_result(&model);
        ui_pause();
        return 0;
}

static void
init_options()
{
        options.time = TIMEOUT;
        options.choices = CHOICES;
        options.players = PLAYERS;
        SAFE_STRNCPY(options.ogg123, OGG123, OPTIONLEN);
        SAFE_STRNCPY(options.encoding, ENCODING, OPTIONLEN);
}

static void
parse_options(int argc, char **argv)
{
        int             ch;

        struct option   longopts[] = {
                {"time", required_argument, NULL, 't'},
                {"choices", required_argument, NULL, 'c'},
                {"players", required_argument, NULL, 'p'},
                {"ogg123", required_argument, NULL, 'o'},
                {"encoding", required_argument, NULL, 'e'},
                {"help", no_argument, NULL, 'h'}
        };

        while ((ch = getopt_long(argc, argv, "t:c:p:o:e:h", longopts, NULL)) != -1)
                switch (ch) {
                case 't':
                        options.time = (int)strtol(optarg, (char **)NULL, 10);
                        break;
                case 'c':
                        options.choices = (int)strtol(optarg, (char **)NULL, 10);
                        if (options.choices < 1 || options.choices > CHOICES)
                                errx(1, "choices must not exceed %d", CHOICES);
                        break;
                case 'p':
                        options.players = (int)strtol(optarg, (char **)NULL, 10);
                        if (options.players < 1 || options.players > PLAYERS)
                                errx(1, "players must not exceed %d", PLAYERS);
                        break;
                case 'o':
                        if (strlen(optarg) >= OPTIONLEN)
                                errx(1, "length of argument 'o' must not exceed %d bytes", OPTIONLEN);
                        SAFE_STRNCPY(options.ogg123, optarg, OPTIONLEN);
                        break;
                case 'e':
                        if (strlen(optarg) >= OPTIONLEN)
                                errx(1, "length of argument 'e' must not exceed %d bytes", OPTIONLEN);
                        SAFE_STRNCPY(options.encoding, optarg, OPTIONLEN);
                        break;
                default:
                case 'h':
                        puts("oggquiz [-t | --time seconds] [-c | --choices choices] [-p | --players players]");
                        puts("        [-o | --ogg123 command] [-e | --encoding encoding]");
                        puts("oggquiz {-h | --help}");
                        exit(0);
                }
}
