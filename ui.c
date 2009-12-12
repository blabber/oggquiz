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
#include <stdlib.h>
#include <unistd.h>

#include "ui.h"

struct ui_context {
        FILE           *in;
        SCREEN         *term;
};

static void     print_header(int *y, char const *step, struct ui_model *model);
static void     print_footer(int *y, char const *instruction);

struct ui_context *
ui_context_open(void)
{
        struct ui_context *ctx;

        if ((ctx = malloc(sizeof(*ctx))) == NULL)
                err(1, "could not malloc ui_context");

        if ((ctx->in = fopen("/dev/tty", "r")) == NULL)
                err(1, "could not fopen /dev/tty");
        if ((ctx->term = newterm(NULL, stdout, ctx->in)) == NULL)
                errx(1, "could not create terminal");
        if (initscr() == NULL)
                errx(1, "could not initialize screen");
        if (set_term(ctx->term) == NULL)
                errx(1, "could not set terminal");

        cbreak();
        noecho();
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        intrflush(stdscr, FALSE);

        return (ctx);
}

void
ui_context_close(struct ui_context *ctx)
{
        assert(ctx != NULL);

        if (fclose(ctx->in) == EOF)
                err(1, "could not fclose /dev/tty");
        delscreen(ctx->term);
        free(ctx);
}

void
ui_display_quiz(struct ui_context *ctx, struct ui_model *model)
{
        int             i;
        int             y = 0;

        assert(ctx != NULL);
        assert(model != NULL);

        clear();

        print_header(&y, "new turn", model);
        y++;

        for (i = 0; i < model->choices; i++) {
                if (i % 2 == 1)
                        attron(A_BOLD);
                else
                        attroff(A_BOLD);
                mvprintw(y++, 1, "%d.  %s\n", i + 1, model->oggfiles[i].artist);
                mvprintw(y++, 1, "    %s\n", model->oggfiles[i].album);
                mvprintw(y++, 1, "    %s\n\n", model->oggfiles[i].title);
                y++;
        }
        attroff(A_BOLD);

        print_footer(&y, "what are you listening to? (# of song, 'q' to quit)");

        refresh();
}

static void
print_header(int *y, char const *step, struct ui_model *model)
{
        assert(step != NULL);
        assert(model != NULL);

        attron(A_REVERSE);
        mvprintw((*y)++, 0, " %s | current turn: %03d | current player: %d ",
                 step, model->turn, model->current_player + 1);
        attroff(A_REVERSE);
}

static void
print_footer(int *y, char const *instruction)
{
        attron(A_REVERSE);
        mvprintw((*y)++, 0, " %s ", instruction);
        attroff(A_REVERSE);

}

void
ui_display_result(struct ui_context *ctx, struct ui_model *model)
{
        int             i;
        int             y = 0;

        assert(ctx != NULL);
        assert(model != NULL);

        clear();

        print_header(&y, "result", model);
        y++;

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
                mvprintw(y++, 3, "%s", model->guess->artist);
                mvprintw(y++, 3, "%s", model->guess->album);
                mvprintw(y++, 3, "%s", model->guess->title);
        }
        attron(COLOR_PAIR(1));

        y++;
        mvprintw(y++, 1, "You are listening to:");
        y++;
        mvprintw(y++, 3, "%s", model->correct->artist);
        mvprintw(y++, 3, "%s", model->correct->album);
        mvprintw(y++, 3, "%s", model->correct->title);
        y++;
        attron(A_BOLD);
        mvprintw(y++, 1, "Scoreboard:");
        attroff(A_BOLD);
        y++;
        for (i = 0; i < model->players; i++) {
                if (i == model->current_player)
                        attron(A_BOLD);
                else
                        attroff(A_BOLD);
                mvprintw(y++, 3, "Player %d: %5d", i + 1, model->scores[i]);
        }
        attrset(A_NORMAL);
        y++;

        print_footer(&y, "press any key to contine, 'q' to quit");

        refresh();
}

char
ui_get_key()
{
        int             in;

        in = getch();

        return ((char)in);
}
