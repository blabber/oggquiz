/*-
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tobias.rehbein@web.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy me a beer in return.
 *                                                              Tobias Rehbein
 */

/**
 * Required includes:
 * oggfile.h
 * oggquiz.h
 */

struct ui_model {
        struct oggfile *oggfiles;
        struct oggfile *guess;
        struct oggfile *correct;
        int             turn;
        int             scores[PLAYERS];
        int             current_player;
};

void            ui_display_quiz(struct ui_model *model, struct options *opts);
void            ui_display_result(struct ui_model *model, struct options *opts);
char            ui_get_answer(void);
void            ui_pause(void);
void            ui_setup(void);
void            ui_teardown(void);
