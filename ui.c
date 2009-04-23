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
#include "common.h"
#include "ui.h"

static void     print_oggfile(oggfile_t * oggfile);

static int      tty = -1;

void
ui_setup()
{
        if (tty == -1)
                if ((tty = open("/dev/tty", O_RDONLY)) == -1)
                        err(1, "could not open /dev/tty");
}

void
ui_display_quiz(oggfile_t * oggfiles)
{
        int             i;

        printf("Next turn:\n");
        for (i = 0; i < CHOICES; i++) {
                printf("%d.  %s\n", i + 1, oggfiles[i].artist);
                printf("    %s\n", oggfiles[i].album);
                printf("    %s\n\n", oggfiles[i].title);
        }
}

void
ui_display_result(ui_result_t * result)
{
        if (result->correct == result->guess)
                printf("\nYou are right!\n");
        else {
                printf("\nSorry, you are wrong!\n");
                printf("\nYour guess:\n");
                print_oggfile(result->guess);
        }

        printf("\nYou are listening to:\n");
        print_oggfile(result->correct);
        puts("");
}

char
ui_get_answer()
{
        int             answer;
        char            canswer;

        printf("What are you listening to? (1-%d, 'q' to quit)\n", CHOICES);
        for (;;) {
                if (read(tty, &answer, 2) == -1)
                        err(1, "could not read from /dev/tty");
                canswer = (char)answer;
                if (canswer == 'q' || (canswer >= '1' && canswer <= '0' + CHOICES))
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
print_oggfile(oggfile_t * oggfile)
{
        printf("  %s\n", oggfile->artist);
        printf("  %s\n", oggfile->album);
        printf("  %s\n", oggfile->title);
}
