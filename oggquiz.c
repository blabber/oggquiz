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
#include "common.h"
#include "oggfile.h"
#include "player.h"
#include "ui.h"

static int      new_turn(oggfile_t * oggfiles);

int
main(int argc, char **argv)
{
        char            filename[FILENAMELEN];
        char           *newline;
        int             oggfileno = 0;
        oggfile_t       oggfiles[CHOICES];

        oggfile_setup();
        ui_setup();

        while (fgets(filename, FILENAMELEN, stdin) != NULL) {
                if ((newline = strchr(filename, '\n')) != NULL)
                        newline[0] = '\0';

                if (!oggfile_create(&oggfiles[oggfileno], filename))
                        oggfileno++;

                if (oggfileno == CHOICES) {
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
                for (correct = 0; correct < CHOICES; correct++) {
                        model.scores[correct] = 0;
                        model.turn = 0;
                }
        }
        model.turn++;
        model.current_player = ((model.turn - 1) % PLAYERS);
        model.oggfiles = oggfiles;
        correct = rand() % 4;
        model.correct = &oggfiles[correct];
        ui_display_quiz(&model);
        player_play(model.correct);
        start = time(NULL);
        guess = ui_get_answer();
        if (guess == 'q')
                return 1;
        model.guess = &oggfiles[guess - '1'];
        if (model.guess == model.correct)
                model.scores[model.current_player] += min(TIMEOUT, time(NULL) - start);
        else
                model.scores[model.current_player] += TIMEOUT;
        ui_display_result(&model);
        ui_pause();
        return 0;
}
