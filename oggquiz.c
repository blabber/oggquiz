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
        static int      rpg_initialized = 0;
        int             answer;
        char            guess;
        ui_result_t     result;

        if (!rpg_initialized) {
                rpg_initialized = 1;
                srand(time(NULL));
        }
        answer = rand() % 4;
        ui_display_quiz(oggfiles);
        player_play(&oggfiles[answer]);
        guess = ui_get_answer();
        if (guess == 'q')
                return 1;
        result.correct = &oggfiles[answer];
        result.guess = &oggfiles[guess - '1'];
        if (result.guess == result.correct)
                /* TODO:  scoring */
                puts("debug: right guess");
        ui_display_result(&result);
        ui_pause();
        return 0;
}
