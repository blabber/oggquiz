/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

#include <assert.h>
#include <curses.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "oggfile.h"
#include "oggquiz.h"
#include "ui.h"

#define RESETGUI()     do {                     \
                                y = 0;          \
                                clear();        \
                        } while (0)

/* Prototypes */
static void     print_oggfile(struct oggfile *oggfile);
static void     print_header(char const *step, struct ui_model *model);

/* Global variables */
static int      tty;
static FILE    *ftty;
static SCREEN  *term;
static int      y;

void
ui_setup()
{
        if ((tty = open("/dev/tty", O_RDONLY)) == -1)
                err(1, "could not open /dev/tty");
        if ((ftty = fopen("/dev/tty", "r")) == NULL)
                err(1, "could not open /dev/tty");
        if ((term = newterm(NULL, stdout, ftty)) == NULL)
                errx(1, "could not create /dev/tty terminal");
        if (initscr() == NULL)
                errx(1, "could not initialize screen");
        if (set_term(term) == NULL)
                errx(1, "could not set /dev/tty terminal");
        cbreak();
        noecho();
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        intrflush(stdscr, FALSE);
}

void
ui_display_quiz(struct ui_model *model, struct options *opts)
{
        int             i;

        assert(model != NULL);
        assert(opts != NULL);

        RESETGUI();
        print_header("new turn", model);

        for (i = 0; i < opts->choices; i++) {
                if (i % 2 == 1) {
                        attron(A_BOLD);
                } else
                        attroff(A_BOLD);
                mvprintw(y++, 1, "%d.  %s\n", i + 1, model->oggfiles[i].artist);
                mvprintw(y++, 1, "    %s\n", model->oggfiles[i].album);
                mvprintw(y++, 1, "    %s\n\n", model->oggfiles[i].title);
                y++;
        }
        attroff(A_BOLD);

        attron(A_REVERSE);
        mvprintw(y, 0, " What are you listening to? (1-%d, 'q' to quit) ", opts->choices);
        attroff(A_REVERSE);

        refresh();
}

void
ui_display_result(struct ui_model *model, struct options *opts)
{
        int             i;

        assert(model != NULL);
        assert(opts != NULL);

        RESETGUI();
        print_header("result", model);

        if (model->correct == model->guess) {
                attron(COLOR_PAIR(2));
                attron(A_BOLD);
                mvprintw(y++, 1, "You are right!");
                attroff(A_BOLD);
        } else {
                attron(COLOR_PAIR(3));
                attron(A_BOLD);
                mvprintw(y++, 1, "Sorry, you are wrong!");
                attroff(A_BOLD);
                y++;
                mvprintw(y++, 1, "Your guess:");
                y++;
                print_oggfile(model->guess);
        }
        attron(COLOR_PAIR(1));

        y++;
        mvprintw(y++, 1, "You are listening to:");
        y++;
        print_oggfile(model->correct);
        y++;
        attron(A_BOLD);
        mvprintw(y++, 1, "Scoreboard:");
        attroff(A_BOLD);
        y++;
        for (i = 0; i < opts->players; i++) {
                if (i == model->current_player)
                        attron(A_BOLD);
                else
                        attroff(A_BOLD);
                mvprintw(y++, 3, "Player %d: %5d", i + 1, model->scores[i]);
        }
        attrset(A_NORMAL);

        attron(A_REVERSE);
        y++;
        mvaddstr(y++, 0, " press any key to continue... ");
        attroff(A_REVERSE);

        refresh();
}

char
ui_get_answer()
{
        int             in;

        in = getch();

        return ((char)in);
}

void
ui_pause()
{
        getch();
}

void
ui_teardown()
{
        if (close(tty) == -1)
                err(1, "could not close fd for /dev/tty");
        if (fclose(ftty) == EOF)
                err(1, "could not close /dev/tty");
        delscreen(term);
}

static void
print_oggfile(struct oggfile *ogg)
{
        assert(ogg != NULL);

        mvprintw(y++, 3, "%s", ogg->artist);
        mvprintw(y++, 3, "%s", ogg->album);
        mvprintw(y++, 3, "%s", ogg->title);
}

static void
print_header(char const *step, struct ui_model *model)
{
        assert(step != NULL);
        assert(model != NULL);

        attron(A_REVERSE);
        mvprintw(y, 0, " %s | current turn: %03d | current player: %d ",
                 step, model->turn, model->current_player + 1);
        y += 2;
        attroff(A_REVERSE);
}
