/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include "oggquiz.h"
#include "common.h"
#include "ui.h"

/* Prototypes */
static void     print_oggfile(oggfile_t *oggfile);

/* Global variables */
extern options_t options;
static int      tty = -1;

void
ui_setup()
{
        if (tty == -1)
                if ((tty = open("/dev/tty", O_RDONLY)) == -1)
                        err(1, "could not open /dev/tty");
}

void
ui_display_quiz(ui_model_t *model)
{
        int             i;

        puts("+-----------+-------------------+-------------------+");
        printf("| Next turn | current turn: %3d | current player: %d |\n",
               model->turn, model->current_player + 1);
        puts("+-----------+-------------------+-------------------+\n");

        for (i = 0; i < options.choices; i++) {
                printf("%d.  %s\n", i + 1, model->oggfiles[i].artist);
                printf("    %s\n", model->oggfiles[i].album);
                printf("    %s\n\n", model->oggfiles[i].title);
        }
}

void
ui_display_result(ui_model_t *model)
{
        int             i;
        char            mark;

        if (model->correct == model->guess)
                printf("\nYou are right!\n");
        else {
                printf("\nSorry, you are wrong!\n");
                printf("\nYour guess:\n");
                print_oggfile(model->guess);
        }

        printf("\nYou are listening to:\n");
        print_oggfile(model->correct);
        puts("\nScoreboard:\n");
        for (i = 0; i < options.players; i++) {
                if (i == model->current_player)
                        mark = '*';
                else
                        mark = ' ';
                printf("%c  Player %d: %5d %c\n", mark, i + 1, model->scores[i], mark);
        }
        puts("");
}

char
ui_get_answer()
{
        int             answer;
        char            canswer;

        printf("What are you listening to? (1-%d, 'q' to quit)\n", options.choices);
        for (;;) {
                if (read(tty, &answer, 2) == -1)
                        err(1, "could not read from /dev/tty");
                canswer = (char)answer;
                if (canswer == 'q' || (canswer >= '1' && canswer <= '0' + options.choices))
                        break;
        }
        return canswer;
}

void
ui_pause()
{
        char            in;

        printf("press return to continue...\n");
        for (;;) {
                if (read(tty, &in, 1) == -1)
                        err(1, "could not read from /dev/tty");
                if (in == '\n')
                        break;
        }
}

void
ui_teardown()
{
        if (tty != -1) {
                if (close(tty) == -1)
                        err(1, "could not close fd for /dev/tty");
                tty = -1;
        }
}

static void
print_oggfile(oggfile_t *oggfile)
{
        printf("  %s\n", oggfile->artist);
        printf("  %s\n", oggfile->album);
        printf("  %s\n", oggfile->title);
}
